using System;
using System.Threading;
using System.Collections.Generic;
using Antmicro.Renode.Core;
using Antmicro.Renode.Peripherals;
using Antmicro.Renode.Peripherals.Bus;
using Antmicro.Renode.Logging;
using Antmicro.Renode.Time;

namespace Antmicro.Renode.Peripherals.DMA
{
    public class STM32F103_DMA : IDoubleWordPeripheral, IKnownSize
    {
        private const int ChannelCount = 7;

        public STM32F103_DMA(Machine machine, IBusController bus)
        {
            this.machine = machine;
            this.bus = bus;

            isr = 0;
            ifcr = 0;

            channels = new Channel[ChannelCount];
            for(int i = 0; i < ChannelCount; i++)
            {
                channels[i] = new Channel(this, i + 1);
            }
        }

        public void Reset()
        {
            isr = 0;
            ifcr = 0;
            foreach(var ch in channels)
                ch.Reset();
        }

        public uint ReadDoubleWord(long offset)
        {
            if (offset == 0x00)
            {
                this.Log(LogLevel.Info, $"=> ISR=0x{isr:X8}");
                return isr;
            }
            if (offset == 0x04) return ifcr;

            var channelIndex = (offset - 0x08) / 0x14;
            var channelOffset = (offset - 0x08) % 0x14;

            if(channelIndex < 0 || channelIndex >= ChannelCount)
                return 0;

            return channels[channelIndex].ReadRegister(channelOffset);
        }

        public void WriteDoubleWord(long offset, uint value)
        {
            if(offset == 0x00)
            {
                // ISR is read-only
                return;
            }

            if(offset == 0x04)
            {
                ifcr = value;
                // Clear bits in ISR where IFCR bits are 1
                isr &= ~value;
                return;
            }

            var channelIndex = (offset - 0x08) / 0x14;
            var channelOffset = (offset - 0x08) % 0x14;

            if(channelIndex < 0 || channelIndex >= ChannelCount)
                return;

            channels[channelIndex].WriteRegister(channelOffset, value);
        }

        public long Size => 0x1C + 0x14 * ChannelCount;

        internal void SetInterrupt(int channel, bool complete)
        {
            var shift = (channel - 1) * 4;
            const uint GIF = 1u;
            const uint TCIF = 1u << 1;

            isr |= (GIF | TCIF) << shift;

            this.Log(LogLevel.Info, $"DMA Channel {channel} transfer complete; ISR=0x{isr:X8}");
            // Optionally trigger an IRQ line here (not implemented yet)
        }

        private uint isr;
        private uint ifcr;

        private readonly Channel[] channels;
        private readonly Machine machine;
        private readonly IBusController bus;

        private class Channel
        {
            public Channel(STM32F103_DMA parent, int channelNumber)
            {
                this.parent = parent;
                this.channelNumber = channelNumber;
                Reset();
            }

            public void Reset()
            {
                ccr = 0;
                cndtr = 0;
                cpar = 0;
                cmar = 0;
            }

            public uint ReadRegister(long offset)
            {
                switch(offset)
                {
                    case 0x00: return ccr;
                    case 0x04: return cndtr;
                    case 0x08: return cpar;
                    case 0x0C: return cmar;
                    case 0x10: return 0; // reserved
                    default:
                        parent.machine.Log(LogLevel.Warning, $"Unhandled DMA channel register read at 0x{offset:X}");
                        return 0;
                }
            }

            public void WriteRegister(long offset, uint value)
            {
                switch(offset)
                {
                    case 0x00:
                        var wasEnabled = (ccr & 0x1) != 0;
                        ccr = value;
                        var isEnabled = (ccr & 0x1) != 0;

                        if(!wasEnabled && isEnabled)
                        {
                            parent.machine.ScheduleAction(TimeInterval.FromMilliseconds(5000), (TimeInterval _) =>
                            {
                                DoTransfer();
                                ccr &= ~0x1u;
                                parent.SetInterrupt(channelNumber, complete: true);
                            });
                        }
                        break;
                    case 0x04: cndtr = value; break;
                    case 0x08: cpar = value; break;
                    case 0x0C: cmar = value; break;
                    default:
                        parent.machine.Log(LogLevel.Warning, $"Unhandled DMA channel register write at 0x{offset:X}");
                        break;
                }
            }

            private void DoTransfer()
            {
                const uint M2M_MASK = 1 << 14;
                const uint DIR_MASK = 1 << 4;

                if((ccr & M2M_MASK) == 0)
                {
                    parent.machine.Log(LogLevel.Warning, $"DMA{channelNumber} not in M2M mode. Aborting transfer.");
                    return;
                }

                var readFromPeripheral = (ccr & DIR_MASK) == 0;
                var source = readFromPeripheral ? cpar : cmar;
                var dest = readFromPeripheral ? cmar : cpar;

                for(uint i = 0; i < cndtr; i++)
                {
                    try
                    {
                        var data = parent.bus.ReadDoubleWord(source + 4 * i);
                        parent.bus.WriteDoubleWord(dest + 4 * i, data);
                        
                        // Thread.Sleep(500); // Simulate a slow DMA
                    }
                    catch (Exception e)
                    {
                        parent.machine.Log(LogLevel.Error, $"DMA{channelNumber} error: {e.Message}");
                        break;
                    }
                }
            }

            private uint ccr, cndtr, cpar, cmar;
            private readonly STM32F103_DMA parent;
            private readonly int channelNumber;
        }
    }
}
