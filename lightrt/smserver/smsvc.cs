using System.Net.Sockets;
using System.Net;
using System.Text;
using System.IO;
using System.Threading;
using System;

namespace testapp
{
    internal class Program
    {
        private static TcpListener listener;
        private const int port = 9000;

        static void Main()
        {
            listener = new TcpListener(IPAddress.Any, port);
            listener.Start();
            Console.WriteLine($"Server started on port {port}...");

            while (true)
            {
                TcpClient client = listener.AcceptTcpClient();
                Console.WriteLine($"Client {client.Client.RemoteEndPoint} connected.");

                Thread clientThread = new Thread(HandleClient);
                clientThread.Start(client);
            }
        }

        static byte[] sharedMemory = new byte[4096];
        static object sharedMemoryLock = new object();

        private static void HandleClient(object clientObject)
        {
            TcpClient client = (TcpClient)clientObject;

            using (NetworkStream stream = client.GetStream())
            using (BinaryReader reader = new BinaryReader(stream))
            using (BinaryWriter writer = new BinaryWriter(stream))
            {
                try
                {
                    while (true)
                    {
                        byte bb = reader.ReadByte();

                        int b = (int)bb;

                        lock (sharedMemoryLock)
                        {
                            long address;

                            switch (b)
                            {
                                case 0:
                                    // Read byte
                                    address = reader.ReadInt64();
                                    {
                                        var data = sharedMemory[address];
                                        writer.Write(data);
                                        writer.Flush();
                                        Console.WriteLine($"{client.Client.RemoteEndPoint} Read {address} [1] -> {data}");
                                    }
                                    break;
                                case 1:
                                    // Read 2 bytes
                                    address = reader.ReadInt64();
                                    {
                                        var data = BitConverter.ToUInt16(sharedMemory, (int)address);
                                        writer.Write(data);
                                        writer.Flush();
                                        Console.WriteLine($"{client.Client.RemoteEndPoint} Read {address} [2] -> {data}");
                                    }
                                    break;
                                case 2:
                                    // Read 4 bytes
                                    address = reader.ReadInt64();
                                    {
                                        var data = BitConverter.ToUInt32(sharedMemory, (int)address);
                                        writer.Write(data);
                                        writer.Flush();
                                        Console.WriteLine($"{client.Client.RemoteEndPoint} Read {address} [4] -> {data}");
                                    }
                                    break;
                                case 10:
                                    // Write byte
                                    address = reader.ReadInt64();
                                    {
                                        var data = reader.ReadByte();
                                        sharedMemory[address] = data;
                                        Console.WriteLine($"{client.Client.RemoteEndPoint} Write {address} [1] <- {data}");
                                    }
                                    break;
                                case 11:
                                    // Write 2 bytes
                                    address = reader.ReadInt64();
                                    {
                                        var data = reader.ReadUInt16();
                                        var bytes = BitConverter.GetBytes(data);
                                        Array.Copy(bytes, 0, sharedMemory, address, bytes.Length);
                                        Console.WriteLine($"{client.Client.RemoteEndPoint} Write {address} [2] <- {data}");
                                    }
                                    break;
                                case 12:
                                    // Write 4 bytes
                                    address = reader.ReadInt64();
                                    Console.WriteLine($"Address = {address}");
                                    {
                                        var data = reader.ReadUInt32();
                                        var bytes = BitConverter.GetBytes(data);
                                        Array.Copy(bytes, 0, sharedMemory, address, bytes.Length);
                                        Console.WriteLine($"{client.Client.RemoteEndPoint} Write {address} [4] <- {data}");
                                    }
                                    break;
                                case 20:
                                    // Clean all
                                    Array.Clear(sharedMemory, 0, sharedMemory.Length);
                                    Console.WriteLine($"{client.Client.RemoteEndPoint} Memory clear");
                                    break;
                                default:
                                    Console.WriteLine($"{client.Client.RemoteEndPoint} Unknown: {bb}");
                                    break;
                            }
                        }
                    }
                }
                catch (IOException)
                {
                    Console.WriteLine("Client disconnected.");
                }
                finally
                {
                    client.Close();
                }
            }
        }
    }
}