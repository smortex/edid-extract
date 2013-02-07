Generate EDID binary dumps by reading an Xorg log file.  The log file must contain a hex EDID dump, this is achieved by enabling *ModeDebug* in the *Monitor* section:

~~~
Section "Monitor"
    ...
    Option "ModeDebug" "TRUE"
EndSection
~~~

Example usage:

~~~
% ./edid-extract /var/log/Xorg.0.log
Found EDID for Samsung SMBX2240
EDID dump written to DFP-0
Found EDID for Samsung SMBX2240
EDID dump written to DFP-1
% ls DFP-*
DFP-0  DFP-1
% hexdump -C DFP-0
00000000  00 ff ff ff ff ff ff 00  4c 2d 84 06 32 32 42 43  |........L-..22BC|
00000010  0e 15 01 03 80 30 1b 78  2a 78 f1 a6 55 48 9b 26  |.....0.x*x..UH.&|
00000020  12 50 54 bf ef 80 71 4f  81 00 81 40 81 80 95 00  |.PT...qO...@....|
00000030  b3 00 a9 40 95 0f 02 3a  80 18 71 38 2d 40 58 2c  |...@...:..q8-@X,|
00000040  45 00 dd 0c 11 00 00 1e  00 00 00 fd 00 32 4b 1e  |E............2K.|
00000050  51 11 00 0a 20 20 20 20  20 20 00 00 00 fc 00 53  |Q...      .....S|
00000060  4d 42 58 32 32 34 30 0a  20 20 20 20 00 00 00 ff  |MBX2240.    ....|
00000070  00 48 39 58 42 34 30 31  30 32 34 0a 20 20 01 b6  |.H9XB401024.  ..|
00000080  02 01 04 00 02 3a 80 d0  72 38 2d 40 10 2c 45 80  |.....:..r8-@.,E.|
00000090  dd 0c 11 00 00 1e 01 1d  00 72 51 d0 1e 20 6e 28  |.........rQ.. n(|
000000a0  55 00 dd 0c 11 00 00 1e  01 1d 00 bc 52 d0 1e 20  |U...........R.. |
000000b0  b8 28 55 40 15 1e 11 00  00 1e 8c 0a d0 90 20 40  |.(U@.......... @|
000000c0  31 20 0c 40 55 00 dd 1e  11 00 00 18 8c 0a d0 8a  |1 .@U...........|
000000d0  20 e0 2d 10 10 3e 96 00  dd 1e 11 00 00 18 00 00  | .-..>..........|
000000e0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
000000f0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 99  |................|
00000100
~~~
