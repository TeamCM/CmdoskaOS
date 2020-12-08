/**
 ** This file is part of BoneOS.
 **
 **   BoneOS is free software: you can redistribute it and/or modify
 **   it under the terms of the GNU General Public License as published by
 **   the Free Software Foundation, either version 3 of the License, or
 **   (at your option) any later version.

 **   BoneOS is distributed in the hope that it will be useful,
 **   but WITHOUT ANY WARRANTY; without even the implied warranty of
 **   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **   GNU General Public License for more details.

 **   You should have received a copy of the GNU General Public License
 **   along with BoneOS.  If not, see <http://www.gnu.org/licenses/>.
 **
 **  @main_author : Amanuel Bogale
 **  
 **  @contributors:

 **     Amanuel Bogale <amanuel2> : start
 **/  

#include <stdint.h>

#define KBD_CTRL_STATS_REG                  0x64
#define KBD_CTRL_CMD_REG                    0x64
#define KBD_CTRL_STATS_MASK_OUT_BUF         0x1
#define KBD_ENCODER_CMD_SEND_SYSTEM_RESET   0xFE
#define KBD_CTRL_STATS_MASK_IN_BUF          0x2
#define KBD_ENC_CMD_REG                     0x60

// Get from BoneOS

uint8_t inb(uint16_t port_num)
{
    uint8_t result;
    __asm__ __volatile__
    (
        "inb %w[port_num],%b[result]\n\t"
        : [result] "=a" (result) // ASM -> Variable
        : [port_num] "Nd" (port_num) // VARIABLE -> ASM
    );

    return result;
}

void outb(uint16_t port_num,uint8_t data)
{
    __asm__ __volatile__
    (
        "outb %b[data],%w[port_num]\n\t"
        :
        : [data] "a" (data),
        [port_num] "Nd" (port_num)
    );
}

uint8_t kbd_ctrl_read_status_reg()
{
    return (inb(KBD_CTRL_STATS_REG));
}

void kbd_ctrl_send_cmd(uint8_t cmd)
{
    while((kbd_ctrl_read_status_reg() & KBD_CTRL_STATS_MASK_IN_BUF) != 0);
    outb(KBD_CTRL_CMD_REG,cmd);
}

void kbd_enc_send_cmd(uint8_t cmd)
{
    while((kbd_ctrl_read_status_reg() & KBD_CTRL_STATS_MASK_IN_BUF) != 0);
    outb(KBD_ENC_CMD_REG,cmd);
}

int reboot_cmdoska(void)
{
    uint8_t _status = 0x02;
    while (_status & 0x02)
        _status = inb(KBD_CTRL_STATS_REG);
    outb(KBD_CTRL_CMD_REG, KBD_ENCODER_CMD_SEND_SYSTEM_RESET);
    kbd_ctrl_send_cmd(KBD_CTRL_STATS_MASK_OUT_BUF);
    kbd_enc_send_cmd(KBD_ENCODER_CMD_SEND_SYSTEM_RESET);
}