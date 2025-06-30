using Antmicro.Renode.Peripherals;
using Antmicro.Renode.Peripherals.Bus;
using Antmicro.Renode.Logging;
using Antmicro.Renode.Core;
using System;
using System.IO;
using System.Net;
using System.Net.Sockets;

namespace Antmicro.Renode.Peripherals.Memory
{
    public class ExternalMemory : IBytePeripheral, IWordPeripheral, IDoubleWordPeripheral
    {
        // private readonly byte[] memory;
        private long Size = 0x1000; // 4 KB memory region

        private TcpClient client;
        private NetworkStream stream;
        private BinaryReader reader;
        private BinaryWriter writer;

        public ExternalMemory(IMachine machine, int size)
        {
            try
            {
                client = new TcpClient("127.0.0.1", 9000);
                stream = client.GetStream();
                writer = new BinaryWriter(stream, System.Text.Encoding.UTF8, leaveOpen: true);
                reader = new BinaryReader(stream, System.Text.Encoding.UTF8, leaveOpen: true);
            }
            catch (Exception e)
            {
                this.Log(LogLevel.Warning, $"Failed to connect to SMServer: {e.Message}");
            }

            this.Log(LogLevel.Info, $"Initialize {size}");

            Size = size;
            // memory = new byte[Size];
        }

        public byte ReadByte(long offset)
        {
            this.Log(LogLevel.Info, $"Read offset: {offset}");

            /*if (offset < 0 || offset >= Size)
                throw new ArgumentOutOfRangeException(nameof(offset));
            return memory[offset];*/

            writer.Write((byte)0);
            writer.Write(offset);
            writer.Flush();

            return reader.ReadByte();
        }

        public ushort ReadWord(long offset)
        {
            this.Log(LogLevel.Info, $"Read word offset: {offset}");

            /*return BitConverter.ToUInt16(memory, (int)offset);*/
            writer.Write((byte)1);
            writer.Write(offset);
            writer.Flush();

            return reader.ReadUInt16();
        }

        public uint ReadDoubleWord(long offset)
        {
            this.Log(LogLevel.Info, $"Read dword offset: {offset}");

            /*return BitConverter.ToUInt32(memory, (int)offset);*/
            writer.Write((byte)2);
            writer.Write(offset);
            writer.Flush();

            return reader.ReadUInt32();
        }

        public void WriteByte(long offset, byte value)
        {
            this.Log(LogLevel.Info, $"Write offset: {offset} = {value}");

            /*if (offset < 0 || offset >= Size)
                throw new ArgumentOutOfRangeException(nameof(offset));
            memory[offset] = value;*/

            writer.Write((byte)10);
            writer.Write(offset);
            writer.Write(value);
            writer.Flush();

            return;
        }

        public void WriteWord(long offset, ushort value)
        {
            /*this.Log(LogLevel.Info, $"Write word offset: {offset} = {value}");

            var bytes = BitConverter.GetBytes(value);
            Array.Copy(bytes, 0, memory, offset, bytes.Length);*/

            writer.Write((byte)11);
            writer.Write(offset);
            writer.Write(value);
            writer.Flush();

            return;
        }

        public void WriteDoubleWord(long offset, uint value)
        {
            this.Log(LogLevel.Info, $"Write dword offset: {offset} = {value}");

            /*var bytes = BitConverter.GetBytes(value);
            Array.Copy(bytes, 0, memory, offset, bytes.Length);*/

            writer.Write((byte)12);
            writer.Write(offset);
            writer.Write(value);
            writer.Flush();

            return;
        }

        public void Reset()
        {
            this.Log(LogLevel.Info, $"Reset");

            /* Array.Clear(memory, 0, memory.Length); */
            writer.Write((byte)20);
            writer.Flush();

            return;
        }
    }
}
