# Remy Maintainer/Dev Docs

## Request Protocol

`protocol "opcode: 8, flag: 8, reserved: 16, packet_len: 32, data (variable): 32"`

```
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|     opcode    |      flag     |            reserved           |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                           packet_len                          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                         data (variable)                       |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

## Response Protocol

`protocol "opcode: 8, reserved: 24, packet_len: 32, data (variable): 32"`

```
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|     opcode    |                    reserved                   |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                           packet_len                          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                         data (variable)                       |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

## Multi-line Packets



## Commands

| Code   | Flags  | Description     |
| ------ | ------ | --------------- |
| `0x01` |        | Status          |
|        | `0x01` | Query status    |
|        | `0x02` | Query uptime    |
| `0x02` |        | File I/O        |
|        | `0x01` | List file       |
|        | `0x02` | Upload file     |
|        | `0x03` | Download file   |
| `0x03` |        | Execute command |
| `0x0F` |        | Interactive CLI |
| `0xFF` | N/A    | Stop remy       |

TODO: Link tunnel and pass onto next machine

## Return

| Return code | Description |
| ----------- | ----------- |
| `0x00`      | Success     |
| `0xFF`      | Failure     |

