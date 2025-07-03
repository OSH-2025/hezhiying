using System.Net.Sockets;
using System.Net;
using System.Text;
using System.IO;
using System.Threading;
using System;

// Change the corresponding syscalls when modify this file.

namespace testapp
{
    internal class Program
    {
        private static TcpListener listener;
        private const int port = 9000;

        static void Main()
        {
            size = 4096;
            sharedMemory = new byte[size];

            Console.WriteLine($"This is LightRT Renode simulated shared memory server, size = {size}");

            listener = new TcpListener(IPAddress.Any, port);
            listener.Start();
            Console.WriteLine($"Server started on port {port}...");

            while (true)
            {
                TcpClient client = listener.AcceptTcpClient();

                Thread clientThread = new Thread(HandleClient);
                clientThread.Start(client);
            }
        }

        static int size = 0;
        static byte[] sharedMemory = null;
        static object sharedMemoryLock = new object();
        static int lockedBy = -1;
        static int specialSize = 8;
        static int sessionID = 0;

        private static void HandleClient(object clientObject)
        {
            TcpClient client = (TcpClient)clientObject;
            int thisSessionID = -1;

            lock (sharedMemoryLock)
            {
                thisSessionID = sessionID;
                sessionID++;
            }

            using (NetworkStream stream = client.GetStream())
            using (BinaryReader reader = new BinaryReader(stream))
            using (BinaryWriter writer = new BinaryWriter(stream))
            {
                Console.WriteLine($"Client {client.Client.RemoteEndPoint} => {thisSessionID} connected.");

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
                                    {
                                        // Read byte
                                        address = reader.ReadInt64();
                                        if (address < specialSize)
                                        {
                                            throw new Exception($"Special mapped address {address} cannot be read in byte mode.");
                                        }
                                        address -= specialSize;
                                        var data = sharedMemory[address];
                                        writer.Write(data);
                                        writer.Flush();
                                        Console.WriteLine($"[{thisSessionID}] Read {address} [1] -> {data}");
                                        break;
                                    }
                                case 1:
                                    {
                                        // Read 2 bytes
                                        address = reader.ReadInt64();
                                        if (address < specialSize)
                                        {
                                            throw new Exception($"Special mapped address {address} cannot be read in 2 byte mode.");
                                        }
                                        address -= specialSize;
                                        var data = BitConverter.ToUInt16(sharedMemory, (int)address);
                                        writer.Write(data);
                                        writer.Flush();
                                        Console.WriteLine($"[{thisSessionID}] Read {address} [2] -> {data}");
                                        break;
                                    }
                                case 2:
                                    {
                                        address = reader.ReadInt64();
                                        // Read 4 bytes
                                        if (address == 0)
                                        {
                                            // Reading special register 0 = size
                                            writer.Write(size);
                                            writer.Flush();
                                        }
                                        else if (address == 4)
                                        {
                                            // Reading special register 4 = lock state
                                            writer.Write(lockedBy == -1 ? 0 : lockedBy == thisSessionID ? 1 : 2);
                                            writer.Flush();
                                        }
                                        else
                                        {
                                            address -= specialSize;
                                            var data = BitConverter.ToUInt32(sharedMemory, (int)address);
                                            writer.Write(data);
                                            writer.Flush();
                                            Console.WriteLine($"[{thisSessionID}] Read {address} [4] -> {data}");
                                        }
                                        break;
                                    }
                                case 10:
                                    {
                                        // Write byte
                                        address = reader.ReadInt64();
                                        if (address < specialSize)
                                        {
                                            throw new Exception($"Special mapped address {address} cannot be written in byte mode.");
                                        }
                                        address -= specialSize;
                                        var data = reader.ReadByte();
                                        sharedMemory[address] = data;
                                        Console.WriteLine($"[{thisSessionID}] Write {address} [1] <- {data}");
                                        break;
                                    }
                                case 11:
                                    {
                                        // Write 2 bytes
                                        address = reader.ReadInt64();
                                        if (address < specialSize)
                                        {
                                            throw new Exception($"Special mapped address {address} cannot be written in 2 byte mode.");
                                        }
                                        address -= specialSize;
                                        var data = reader.ReadUInt16();
                                        var bytes = BitConverter.GetBytes(data);
                                        Array.Copy(bytes, 0, sharedMemory, address, bytes.Length);
                                        Console.WriteLine($"[{thisSessionID}] Write {address} [2] <- {data}");
                                        break;
                                    }
                                case 12:
                                    {
                                        // Write 4 bytes
                                        address = reader.ReadInt64();
                                        if (address == 0)
                                        {
                                            // Writing special register 0 = size
                                            throw new Exception($"Offset 0 is size constant and cannot be written to.");
                                        }
                                        else if (address == 4)
                                        {
                                            // Console.WriteLine("Write to addr 4");
                                            var d = reader.ReadUInt32();

                                            // Reading special register 4 = lock state
                                            if (lockedBy == -1)
                                            {
                                                // Unlocked
                                                if (d == 1)
                                                {
                                                    lockedBy = thisSessionID;
                                                    Console.WriteLine($"Memory locked by {thisSessionID}.");
                                                }
                                            }
                                            else
                                            {
                                                // Locked
                                                if (lockedBy != thisSessionID)
                                                {
                                                    // Do nothing because the memory is not locked by this node
                                                }
                                                else
                                                {
                                                    if (d == 0)
                                                    {
                                                        lockedBy = -1;
                                                        Console.WriteLine($"Memory unlocked.");
                                                    }
                                                    else
                                                    {
                                                        // Ignore invalid value.
                                                    }
                                                }
                                            }
                                            // Console.WriteLine("Special write done");
                                        }
                                        else
                                        {
                                            address -= specialSize;
                                            // Console.WriteLine($"Address = {address}");
                                            var data = reader.ReadUInt32();
                                            var bytes = BitConverter.GetBytes(data);
                                            Array.Copy(bytes, 0, sharedMemory, address, bytes.Length);
                                            Console.WriteLine($"[{thisSessionID}] Write {address} [4] <- {data}");
                                        }
                                        break;
                                    }
                                case 20:
                                    {
                                        // Clean all
                                        Array.Clear(sharedMemory, 0, sharedMemory.Length);
                                        Console.WriteLine($"[{thisSessionID}] Memory clear");
                                        break;
                                    }
                                default:
                                    {
                                        throw new Exception($"Invalid opcode {b}");
                                        // break;
                                    }
                            }
                        }
                    }
                }
                catch (IOException)
                {
                    Console.WriteLine("Client disconnected.");
                }
                catch (Exception ex)
                {
                    Console.Write(ex.ToString());
                }
                finally
                {
                    client.Close();
                }
            }
        }
    }
}