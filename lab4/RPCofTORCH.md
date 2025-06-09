## PyTorch RPC 分布式部署方案

本次部署基于 **PyTorch 的 `torch.distributed.rpc` 模块**，实现了在本地 Windows 上的 NVIDIA GeForce RTX 4060 显卡与远程服务器上的 NVIDIA L20 GPU 之间进行跨设备通信与推理调用。

该部署方案中，**本地作为客户端（worker0）**，负责发起远程调用；**远程服务器作为服务端（worker1）**，运行 Qwen 模型并执行推理任务。模型验证数据集为 OKVQA 数据集的验证集，包含 5046 个图文对。

---

##  部署前提条件

| 条件 | 说明                                                 |
|------|----------------------------------------------------|
| 环境一致性 | 本地和远程服务器均需安装相同版本的 PyTorch 及相关依赖                    |
| 网络互通 | 本地与远程服务器之间可通过公网 IP 或 DDNS 域名互相访问，并开放指定端口（如 29500）  |
| 模型准备 | 远程服务器需预加载 Qwen 模型及 OKVQA 数据集验证集 ，这部分可以参考在部署文档部分的说明 |

---

## ️ 部署流程详解

---
### 1. 本地与远程环境配置

本地和服务器两台机器都已安装 PyTorch

---
### 2.RPC调用脚本
服务端代码如下，具体函数可以参考部署文档部分对模型和数据集的封装类
```python
import torch
import torch.distributed.rpc as rpc
from modelscope import Qwen2_5_VLForConditionalGeneration, AutoProcessor, AutoTokenizer
from qwen_vl_utils import process_vision_info

class RemoteModelService:
def __init__(self):
# Load the model and processor
self.model = Qwen2_5_VLForConditionalGeneration.from_pretrained(
"Qwen/Qwen2.5-VL-7B-Instruct", torch_dtype="auto", device_map="auto"
)
self.processor = AutoProcessor.from_pretrained("Qwen/Qwen2.5-VL-7B-Instruct")

    def inf_with_messages(self, messages):
        text = self.processor.apply_chat_template(messages, tokenize=False, add_generation_prompt=True)
        image_inputs, _ = process_vision_info(messages)
        inputs = self.processor(text=[text], images=image_inputs, padding=True, return_tensors="pt")
        inputs = inputs.to("cuda")
        generated_ids = self.model.generate(**inputs, max_new_tokens=128)
        output_text = self.processor.batch_decode(generated_ids, skip_special_tokens=True)
        return output_text[0]

def run_server():
rpc_backend_options = rpc.TensorPipeRpcBackendOptions()
rpc.init_rpc("RemoteModelServer", rank=0, world_size=1, rpc_backend_options=rpc_backend_options)
rref = rpc.remote("RemoteModelServer", RemoteModelService)
rpc.shutdown()

if __name__ == "__main__":
run_server()
```
本地主机代码如下，主要负责构造图文请求、调用远程服务端的 RPC 方法，并保存输出结果。
```python
import json
import tqdm
import os
import torch.distributed.rpc as rpc
from src.fileloader.google import datas

def generate(model_rref, dataset, outputdir):
    """
    使用给定的模型对数据集中的每个条目进行推理，并将结果保存为jsonl格式的文件。
    :param model_rref: 远程模型的RRef实例。
    :param dataset: 数据集实例。
    :param outputdir: 输出目录路径。
    """
    with open(outputdir, 'w', encoding='utf-8') as f:
        for each in tqdm.tqdm(dataset.combined, desc="Processing images"):
            id = each["id"]
            image_path = each["image_path"]
            question = each["question"]
            messages = [
                {"role": "user",
                 "content": [{"type": "image", "image": image_path}, {"type": "text", "text": question}]}
            ]
            result = rpc.rpc_sync("RemoteModelServer", model_rref.owner().inf_with_messages, args=(messages,))
            output_dict = {
                "id": id,
                "question": question,
                "answer": result
            }

            f.write(json.dumps(output_dict, ensure_ascii=False) + "\n")
            f.flush()

if __name__ == "__main__":
    datapath = "./data/OKVQA"
    outputdir = "./gemma_basic.jsonl"  # 指定输出目录
    dataset = datas(datapath)

    rpc_backend_options = rpc.TensorPipeRpcBackendOptions()
    rpc.init_rpc("Client", rank=1, world_size=2, rpc_backend_options=rpc_backend_options)

    model_rref = rpc.remote("RemoteModelServer", RemoteModelService)
    generate(model_rref=model_rref, dataset=dataset, outputdir=outputdir)

    rpc.shutdown()
```

### 脚本执行
**在服务端**，执行下述代码。其中Windows公网ip通过校园网ustcnet获得，yizhilouyi.top是我购买的顶级域名，hezhiying.yizhilouyi.top是通过ddns绑定ip的域名
```shell
python server.py --master_addr hezhiying.yizhilouyi.top --master_port 29500 --rank 0 --world_size 2
```

**在主机端**，执行下述代码,隐私打码处理
```shell
python client.py --master_addr **** --master_port 29500 --rank 1 --world_size 2
```
接着执行代码即可

---

## 测试效果
本地对图片和部分信息进行了预处理，但是实际上大部分还都是客户端进行了对应的处理。包括图片的归一化和计算，以及对应tokens的生成。略有加速但是实际效果并没有提升多少，尤其是服务端还要等待主机端的通信，也会略微降低速度。

