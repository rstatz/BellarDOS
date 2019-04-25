extern interrupt_handler
extern interrupt_handler_err

global isr_unsupported

global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31
global isr32
global isr33
global isr34
global isr35
global isr36
global isr37
global isr38
global isr39
global isr40
global isr41
global isr42
global isr43
global isr44
global isr45
global isr46
global isr47
global isr48
global isr49
global isr50
global isr51
global isr52
global isr53
global isr54
global isr55
global isr56
global isr57
global isr58
global isr59
global isr60
global isr61
global isr62
global isr63
global isr64
global isr65
global isr66
global isr67
global isr68
global isr69
global isr70
global isr71
global isr72
global isr73
global isr74
global isr75
global isr76
global isr77
global isr78
global isr79
global isr80
global isr81
global isr82
global isr83
global isr84
global isr85
global isr86
global isr87
global isr88
global isr89
global isr90
global isr91
global isr92
global isr93
global isr94
global isr95
global isr96
global isr97
global isr98
global isr99
global isr100
global isr101
global isr102
global isr103
global isr104
global isr105
global isr106
global isr107
global isr108
global isr109
global isr110
global isr111
global isr112
global isr113
global isr114
global isr115
global isr116
global isr117
global isr118
global isr119
global isr120
global isr121
global isr122
global isr123
global isr124
global isr125
global isr126
global isr127
global isr128
global isr129
global isr130
global isr131
global isr132
global isr133
global isr134
global isr135
global isr136
global isr137
global isr138
global isr139
global isr140
global isr141
global isr142
global isr143
global isr144
global isr145
global isr146
global isr147
global isr148
global isr149
global isr150
global isr151
global isr152
global isr153
global isr154
global isr155
global isr156
global isr157
global isr158
global isr159
global isr160
global isr161
global isr162
global isr163
global isr164
global isr165
global isr166
global isr167
global isr168
global isr169
global isr170
global isr171
global isr172
global isr173
global isr174
global isr175
global isr176
global isr177
global isr178
global isr179
global isr180
global isr181
global isr182
global isr183
global isr184
global isr185
global isr186
global isr187
global isr188
global isr189
global isr190
global isr191
global isr192
global isr193
global isr194
global isr195
global isr196
global isr197
global isr198
global isr199
global isr200
global isr201
global isr202
global isr203
global isr204
global isr205
global isr206
global isr207
global isr208
global isr209
global isr210
global isr211
global isr212
global isr213
global isr214
global isr215
global isr216
global isr217
global isr218
global isr219
global isr220
global isr221
global isr222
global isr223
global isr224
global isr225
global isr226
global isr227
global isr228
global isr229
global isr230
global isr231
global isr232
global isr233
global isr234
global isr235
global isr236
global isr237
global isr238
global isr239
global isr240
global isr241
global isr242
global isr243
global isr244
global isr245
global isr246
global isr247
global isr248
global isr249
global isr250
global isr251
global isr252
global isr253
global isr254
global isr255

isr_normal:
      hlt
      call interrupt_handler
      pop rdi
      iretq

isr_err:
      hlt
      call interrupt_handler_err
      pop rsi
      pop rdi
      add rsp, 8
      iretq

isr0:
      cli
      push rdi
      mov rdi, 0x0
      jmp isr_normal

isr1:
      cli
      push rdi
      mov rdi, 0x1
      jmp isr_normal

isr2:
      cli
      push rdi
      mov rdi, 0x2
      jmp isr_normal

isr3:
      cli
      push rdi
      mov rdi, 0x3
      jmp isr_normal

isr4:
      cli
      push rdi
      mov rdi, 0x4
      jmp isr_normal

isr5:
      cli
      push rdi
      mov rdi, 0x5
      jmp isr_normal

isr6:
      cli
      push rdi
      mov rdi, 0x6
      jmp isr_normal

isr7:
      cli
      push rdi
      mov rdi, 0x7
      jmp isr_normal

isr8:
      cli
      push rdi
      push rsi
      mov rdi, 0x8
      mov rsi, qword [rsp + 16]
      jmp isr_err

isr9:
      cli
      push rdi
      mov rdi, 0x9
      jmp isr_normal

isr10:
      cli
      push rdi
      push rsi
      mov rdi, 0xa
      mov rsi, qword [rsp + 16]
      jmp isr_err

isr11:
      cli
      push rdi
      push rsi
      mov rdi, 0xb
      mov rsi, qword [rsp + 16]
      jmp isr_err

isr12:
      cli
      push rdi
      push rsi
      mov rdi, 0xc
      mov rsi, qword [rsp + 16]
      jmp isr_err

isr13:
      cli
      push rdi
      push rsi
      mov rdi, 0xd
      mov rsi, qword [rsp + 16]
      jmp isr_err

isr14:
      cli
      push rdi
      push rsi
      mov rdi, 0xe
      mov rsi, qword [rsp + 16]
      jmp isr_err

isr15:
      cli
      push rdi
      mov rdi, 0xf
      jmp isr_normal

isr16:
      cli
      push rdi
      mov rdi, 0x10
      jmp isr_normal

isr17:
      cli
      push rdi
      push rsi
      mov rdi, 0x11
      mov rsi, qword [rsp + 16]
      jmp isr_err

isr18:
      cli
      push rdi
      mov rdi, 0x12
      jmp isr_normal

isr19:
      cli
      push rdi
      mov rdi, 0x13
      jmp isr_normal

isr20:
      cli
      push rdi
      mov rdi, 0x14
      jmp isr_normal

isr21:
      cli
      push rdi
      mov rdi, 0x15
      jmp isr_normal

isr22:
      cli
      push rdi
      mov rdi, 0x16
      jmp isr_normal

isr23:
      cli
      push rdi
      mov rdi, 0x17
      jmp isr_normal

isr24:
      cli
      push rdi
      mov rdi, 0x18
      jmp isr_normal

isr25:
      cli
      push rdi
      mov rdi, 0x19
      jmp isr_normal

isr26:
      cli
      push rdi
      mov rdi, 0x1a
      jmp isr_normal

isr27:
      cli
      push rdi
      mov rdi, 0x1b
      jmp isr_normal

isr28:
      cli
      push rdi
      mov rdi, 0x1c
      jmp isr_normal

isr29:
      cli
      push rdi
      mov rdi, 0x1d
      jmp isr_normal

isr30:
      cli
      push rdi
      push rsi
      mov rdi, 0x1e
      mov rsi, qword [rsp + 16]
      jmp isr_err

isr31:
      cli
      push rdi
      mov rdi, 0x1f
      jmp isr_normal

isr32:
      cli
      push rdi
      mov rdi, 0x20
      jmp isr_normal

isr33:
      cli
      push rdi
      mov rdi, 0x21
      jmp isr_normal

isr34:
      cli
      push rdi
      mov rdi, 0x22
      jmp isr_normal

isr35:
      cli
      push rdi
      mov rdi, 0x23
      jmp isr_normal

isr36:
      cli
      push rdi
      mov rdi, 0x24
      jmp isr_normal

isr37:
      cli
      push rdi
      mov rdi, 0x25
      jmp isr_normal

isr38:
      cli
      push rdi
      mov rdi, 0x26
      jmp isr_normal

isr39:
      cli
      push rdi
      mov rdi, 0x27
      jmp isr_normal

isr40:
      cli
      push rdi
      mov rdi, 0x28
      jmp isr_normal

isr41:
      cli
      push rdi
      mov rdi, 0x29
      jmp isr_normal

isr42:
      cli
      push rdi
      mov rdi, 0x2a
      jmp isr_normal

isr43:
      cli
      push rdi
      mov rdi, 0x2b
      jmp isr_normal

isr44:
      cli
      push rdi
      mov rdi, 0x2c
      jmp isr_normal

isr45:
      cli
      push rdi
      mov rdi, 0x2d
      jmp isr_normal

isr46:
      cli
      push rdi
      mov rdi, 0x2e
      jmp isr_normal

isr47:
      cli
      push rdi
      mov rdi, 0x2f
      jmp isr_normal

isr48:
      cli
      push rdi
      mov rdi, 0x30
      jmp isr_normal

isr49:
      cli
      push rdi
      mov rdi, 0x31
      jmp isr_normal

isr50:
      cli
      push rdi
      mov rdi, 0x32
      jmp isr_normal

isr51:
      cli
      push rdi
      mov rdi, 0x33
      jmp isr_normal

isr52:
      cli
      push rdi
      mov rdi, 0x34
      jmp isr_normal

isr53:
      cli
      push rdi
      mov rdi, 0x35
      jmp isr_normal

isr54:
      cli
      push rdi
      mov rdi, 0x36
      jmp isr_normal

isr55:
      cli
      push rdi
      mov rdi, 0x37
      jmp isr_normal

isr56:
      cli
      push rdi
      mov rdi, 0x38
      jmp isr_normal

isr57:
      cli
      push rdi
      mov rdi, 0x39
      jmp isr_normal

isr58:
      cli
      push rdi
      mov rdi, 0x3a
      jmp isr_normal

isr59:
      cli
      push rdi
      mov rdi, 0x3b
      jmp isr_normal

isr60:
      cli
      push rdi
      mov rdi, 0x3c
      jmp isr_normal

isr61:
      cli
      push rdi
      mov rdi, 0x3d
      jmp isr_normal

isr62:
      cli
      push rdi
      mov rdi, 0x3e
      jmp isr_normal

isr63:
      cli
      push rdi
      mov rdi, 0x3f
      jmp isr_normal

isr64:
      cli
      push rdi
      mov rdi, 0x40
      jmp isr_normal

isr65:
      cli
      push rdi
      mov rdi, 0x41
      jmp isr_normal

isr66:
      cli
      push rdi
      mov rdi, 0x42
      jmp isr_normal

isr67:
      cli
      push rdi
      mov rdi, 0x43
      jmp isr_normal

isr68:
      cli
      push rdi
      mov rdi, 0x44
      jmp isr_normal

isr69:
      cli
      push rdi
      mov rdi, 0x45
      jmp isr_normal

isr70:
      cli
      push rdi
      mov rdi, 0x46
      jmp isr_normal

isr71:
      cli
      push rdi
      mov rdi, 0x47
      jmp isr_normal

isr72:
      cli
      push rdi
      mov rdi, 0x48
      jmp isr_normal

isr73:
      cli
      push rdi
      mov rdi, 0x49
      jmp isr_normal

isr74:
      cli
      push rdi
      mov rdi, 0x4a
      jmp isr_normal

isr75:
      cli
      push rdi
      mov rdi, 0x4b
      jmp isr_normal

isr76:
      cli
      push rdi
      mov rdi, 0x4c
      jmp isr_normal

isr77:
      cli
      push rdi
      mov rdi, 0x4d
      jmp isr_normal

isr78:
      cli
      push rdi
      mov rdi, 0x4e
      jmp isr_normal

isr79:
      cli
      push rdi
      mov rdi, 0x4f
      jmp isr_normal

isr80:
      cli
      push rdi
      mov rdi, 0x50
      jmp isr_normal

isr81:
      cli
      push rdi
      mov rdi, 0x51
      jmp isr_normal

isr82:
      cli
      push rdi
      mov rdi, 0x52
      jmp isr_normal

isr83:
      cli
      push rdi
      mov rdi, 0x53
      jmp isr_normal

isr84:
      cli
      push rdi
      mov rdi, 0x54
      jmp isr_normal

isr85:
      cli
      push rdi
      mov rdi, 0x55
      jmp isr_normal

isr86:
      cli
      push rdi
      mov rdi, 0x56
      jmp isr_normal

isr87:
      cli
      push rdi
      mov rdi, 0x57
      jmp isr_normal

isr88:
      cli
      push rdi
      mov rdi, 0x58
      jmp isr_normal

isr89:
      cli
      push rdi
      mov rdi, 0x59
      jmp isr_normal

isr90:
      cli
      push rdi
      mov rdi, 0x5a
      jmp isr_normal

isr91:
      cli
      push rdi
      mov rdi, 0x5b
      jmp isr_normal

isr92:
      cli
      push rdi
      mov rdi, 0x5c
      jmp isr_normal

isr93:
      cli
      push rdi
      mov rdi, 0x5d
      jmp isr_normal

isr94:
      cli
      push rdi
      mov rdi, 0x5e
      jmp isr_normal

isr95:
      cli
      push rdi
      mov rdi, 0x5f
      jmp isr_normal

isr96:
      cli
      push rdi
      mov rdi, 0x60
      jmp isr_normal

isr97:
      cli
      push rdi
      mov rdi, 0x61
      jmp isr_normal

isr98:
      cli
      push rdi
      mov rdi, 0x62
      jmp isr_normal

isr99:
      cli
      push rdi
      mov rdi, 0x63
      jmp isr_normal

isr100:
      cli
      push rdi
      mov rdi, 0x64
      jmp isr_normal

isr101:
      cli
      push rdi
      mov rdi, 0x65
      jmp isr_normal

isr102:
      cli
      push rdi
      mov rdi, 0x66
      jmp isr_normal

isr103:
      cli
      push rdi
      mov rdi, 0x67
      jmp isr_normal

isr104:
      cli
      push rdi
      mov rdi, 0x68
      jmp isr_normal

isr105:
      cli
      push rdi
      mov rdi, 0x69
      jmp isr_normal

isr106:
      cli
      push rdi
      mov rdi, 0x6a
      jmp isr_normal

isr107:
      cli
      push rdi
      mov rdi, 0x6b
      jmp isr_normal

isr108:
      cli
      push rdi
      mov rdi, 0x6c
      jmp isr_normal

isr109:
      cli
      push rdi
      mov rdi, 0x6d
      jmp isr_normal

isr110:
      cli
      push rdi
      mov rdi, 0x6e
      jmp isr_normal

isr111:
      cli
      push rdi
      mov rdi, 0x6f
      jmp isr_normal

isr112:
      cli
      push rdi
      mov rdi, 0x70
      jmp isr_normal

isr113:
      cli
      push rdi
      mov rdi, 0x71
      jmp isr_normal

isr114:
      cli
      push rdi
      mov rdi, 0x72
      jmp isr_normal

isr115:
      cli
      push rdi
      mov rdi, 0x73
      jmp isr_normal

isr116:
      cli
      push rdi
      mov rdi, 0x74
      jmp isr_normal

isr117:
      cli
      push rdi
      mov rdi, 0x75
      jmp isr_normal

isr118:
      cli
      push rdi
      mov rdi, 0x76
      jmp isr_normal

isr119:
      cli
      push rdi
      mov rdi, 0x77
      jmp isr_normal

isr120:
      cli
      push rdi
      mov rdi, 0x78
      jmp isr_normal

isr121:
      cli
      push rdi
      mov rdi, 0x79
      jmp isr_normal

isr122:
      cli
      push rdi
      mov rdi, 0x7a
      jmp isr_normal

isr123:
      cli
      push rdi
      mov rdi, 0x7b
      jmp isr_normal

isr124:
      cli
      push rdi
      mov rdi, 0x7c
      jmp isr_normal

isr125:
      cli
      push rdi
      mov rdi, 0x7d
      jmp isr_normal

isr126:
      cli
      push rdi
      mov rdi, 0x7e
      jmp isr_normal

isr127:
      cli
      push rdi
      mov rdi, 0x7f
      jmp isr_normal

isr128:
      cli
      push rdi
      mov rdi, 0x80
      jmp isr_normal

isr129:
      cli
      push rdi
      mov rdi, 0x81
      jmp isr_normal

isr130:
      cli
      push rdi
      mov rdi, 0x82
      jmp isr_normal

isr131:
      cli
      push rdi
      mov rdi, 0x83
      jmp isr_normal

isr132:
      cli
      push rdi
      mov rdi, 0x84
      jmp isr_normal

isr133:
      cli
      push rdi
      mov rdi, 0x85
      jmp isr_normal

isr134:
      cli
      push rdi
      mov rdi, 0x86
      jmp isr_normal

isr135:
      cli
      push rdi
      mov rdi, 0x87
      jmp isr_normal

isr136:
      cli
      push rdi
      mov rdi, 0x88
      jmp isr_normal

isr137:
      cli
      push rdi
      mov rdi, 0x89
      jmp isr_normal

isr138:
      cli
      push rdi
      mov rdi, 0x8a
      jmp isr_normal

isr139:
      cli
      push rdi
      mov rdi, 0x8b
      jmp isr_normal

isr140:
      cli
      push rdi
      mov rdi, 0x8c
      jmp isr_normal

isr141:
      cli
      push rdi
      mov rdi, 0x8d
      jmp isr_normal

isr142:
      cli
      push rdi
      mov rdi, 0x8e
      jmp isr_normal

isr143:
      cli
      push rdi
      mov rdi, 0x8f
      jmp isr_normal

isr144:
      cli
      push rdi
      mov rdi, 0x90
      jmp isr_normal

isr145:
      cli
      push rdi
      mov rdi, 0x91
      jmp isr_normal

isr146:
      cli
      push rdi
      mov rdi, 0x92
      jmp isr_normal

isr147:
      cli
      push rdi
      mov rdi, 0x93
      jmp isr_normal

isr148:
      cli
      push rdi
      mov rdi, 0x94
      jmp isr_normal

isr149:
      cli
      push rdi
      mov rdi, 0x95
      jmp isr_normal

isr150:
      cli
      push rdi
      mov rdi, 0x96
      jmp isr_normal

isr151:
      cli
      push rdi
      mov rdi, 0x97
      jmp isr_normal

isr152:
      cli
      push rdi
      mov rdi, 0x98
      jmp isr_normal

isr153:
      cli
      push rdi
      mov rdi, 0x99
      jmp isr_normal

isr154:
      cli
      push rdi
      mov rdi, 0x9a
      jmp isr_normal

isr155:
      cli
      push rdi
      mov rdi, 0x9b
      jmp isr_normal

isr156:
      cli
      push rdi
      mov rdi, 0x9c
      jmp isr_normal

isr157:
      cli
      push rdi
      mov rdi, 0x9d
      jmp isr_normal

isr158:
      cli
      push rdi
      mov rdi, 0x9e
      jmp isr_normal

isr159:
      cli
      push rdi
      mov rdi, 0x9f
      jmp isr_normal

isr160:
      cli
      push rdi
      mov rdi, 0xa0
      jmp isr_normal

isr161:
      cli
      push rdi
      mov rdi, 0xa1
      jmp isr_normal

isr162:
      cli
      push rdi
      mov rdi, 0xa2
      jmp isr_normal

isr163:
      cli
      push rdi
      mov rdi, 0xa3
      jmp isr_normal

isr164:
      cli
      push rdi
      mov rdi, 0xa4
      jmp isr_normal

isr165:
      cli
      push rdi
      mov rdi, 0xa5
      jmp isr_normal

isr166:
      cli
      push rdi
      mov rdi, 0xa6
      jmp isr_normal

isr167:
      cli
      push rdi
      mov rdi, 0xa7
      jmp isr_normal

isr168:
      cli
      push rdi
      mov rdi, 0xa8
      jmp isr_normal

isr169:
      cli
      push rdi
      mov rdi, 0xa9
      jmp isr_normal

isr170:
      cli
      push rdi
      mov rdi, 0xaa
      jmp isr_normal

isr171:
      cli
      push rdi
      mov rdi, 0xab
      jmp isr_normal

isr172:
      cli
      push rdi
      mov rdi, 0xac
      jmp isr_normal

isr173:
      cli
      push rdi
      mov rdi, 0xad
      jmp isr_normal

isr174:
      cli
      push rdi
      mov rdi, 0xae
      jmp isr_normal

isr175:
      cli
      push rdi
      mov rdi, 0xaf
      jmp isr_normal

isr176:
      cli
      push rdi
      mov rdi, 0xb0
      jmp isr_normal

isr177:
      cli
      push rdi
      mov rdi, 0xb1
      jmp isr_normal

isr178:
      cli
      push rdi
      mov rdi, 0xb2
      jmp isr_normal

isr179:
      cli
      push rdi
      mov rdi, 0xb3
      jmp isr_normal

isr180:
      cli
      push rdi
      mov rdi, 0xb4
      jmp isr_normal

isr181:
      cli
      push rdi
      mov rdi, 0xb5
      jmp isr_normal

isr182:
      cli
      push rdi
      mov rdi, 0xb6
      jmp isr_normal

isr183:
      cli
      push rdi
      mov rdi, 0xb7
      jmp isr_normal

isr184:
      cli
      push rdi
      mov rdi, 0xb8
      jmp isr_normal

isr185:
      cli
      push rdi
      mov rdi, 0xb9
      jmp isr_normal

isr186:
      cli
      push rdi
      mov rdi, 0xba
      jmp isr_normal

isr187:
      cli
      push rdi
      mov rdi, 0xbb
      jmp isr_normal

isr188:
      cli
      push rdi
      mov rdi, 0xbc
      jmp isr_normal

isr189:
      cli
      push rdi
      mov rdi, 0xbd
      jmp isr_normal

isr190:
      cli
      push rdi
      mov rdi, 0xbe
      jmp isr_normal

isr191:
      cli
      push rdi
      mov rdi, 0xbf
      jmp isr_normal

isr192:
      cli
      push rdi
      mov rdi, 0xc0
      jmp isr_normal

isr193:
      cli
      push rdi
      mov rdi, 0xc1
      jmp isr_normal

isr194:
      cli
      push rdi
      mov rdi, 0xc2
      jmp isr_normal

isr195:
      cli
      push rdi
      mov rdi, 0xc3
      jmp isr_normal

isr196:
      cli
      push rdi
      mov rdi, 0xc4
      jmp isr_normal

isr197:
      cli
      push rdi
      mov rdi, 0xc5
      jmp isr_normal

isr198:
      cli
      push rdi
      mov rdi, 0xc6
      jmp isr_normal

isr199:
      cli
      push rdi
      mov rdi, 0xc7
      jmp isr_normal

isr200:
      cli
      push rdi
      mov rdi, 0xc8
      jmp isr_normal

isr201:
      cli
      push rdi
      mov rdi, 0xc9
      jmp isr_normal

isr202:
      cli
      push rdi
      mov rdi, 0xca
      jmp isr_normal

isr203:
      cli
      push rdi
      mov rdi, 0xcb
      jmp isr_normal

isr204:
      cli
      push rdi
      mov rdi, 0xcc
      jmp isr_normal

isr205:
      cli
      push rdi
      mov rdi, 0xcd
      jmp isr_normal

isr206:
      cli
      push rdi
      mov rdi, 0xce
      jmp isr_normal

isr207:
      cli
      push rdi
      mov rdi, 0xcf
      jmp isr_normal

isr208:
      cli
      push rdi
      mov rdi, 0xd0
      jmp isr_normal

isr209:
      cli
      push rdi
      mov rdi, 0xd1
      jmp isr_normal

isr210:
      cli
      push rdi
      mov rdi, 0xd2
      jmp isr_normal

isr211:
      cli
      push rdi
      mov rdi, 0xd3
      jmp isr_normal

isr212:
      cli
      push rdi
      mov rdi, 0xd4
      jmp isr_normal

isr213:
      cli
      push rdi
      mov rdi, 0xd5
      jmp isr_normal

isr214:
      cli
      push rdi
      mov rdi, 0xd6
      jmp isr_normal

isr215:
      cli
      push rdi
      mov rdi, 0xd7
      jmp isr_normal

isr216:
      cli
      push rdi
      mov rdi, 0xd8
      jmp isr_normal

isr217:
      cli
      push rdi
      mov rdi, 0xd9
      jmp isr_normal

isr218:
      cli
      push rdi
      mov rdi, 0xda
      jmp isr_normal

isr219:
      cli
      push rdi
      mov rdi, 0xdb
      jmp isr_normal

isr220:
      cli
      push rdi
      mov rdi, 0xdc
      jmp isr_normal

isr221:
      cli
      push rdi
      mov rdi, 0xdd
      jmp isr_normal

isr222:
      cli
      push rdi
      mov rdi, 0xde
      jmp isr_normal

isr223:
      cli
      push rdi
      mov rdi, 0xdf
      jmp isr_normal

isr224:
      cli
      push rdi
      mov rdi, 0xe0
      jmp isr_normal

isr225:
      cli
      push rdi
      mov rdi, 0xe1
      jmp isr_normal

isr226:
      cli
      push rdi
      mov rdi, 0xe2
      jmp isr_normal

isr227:
      cli
      push rdi
      mov rdi, 0xe3
      jmp isr_normal

isr228:
      cli
      push rdi
      mov rdi, 0xe4
      jmp isr_normal

isr229:
      cli
      push rdi
      mov rdi, 0xe5
      jmp isr_normal

isr230:
      cli
      push rdi
      mov rdi, 0xe6
      jmp isr_normal

isr231:
      cli
      push rdi
      mov rdi, 0xe7
      jmp isr_normal

isr232:
      cli
      push rdi
      mov rdi, 0xe8
      jmp isr_normal

isr233:
      cli
      push rdi
      mov rdi, 0xe9
      jmp isr_normal

isr234:
      cli
      push rdi
      mov rdi, 0xea
      jmp isr_normal

isr235:
      cli
      push rdi
      mov rdi, 0xeb
      jmp isr_normal

isr236:
      cli
      push rdi
      mov rdi, 0xec
      jmp isr_normal

isr237:
      cli
      push rdi
      mov rdi, 0xed
      jmp isr_normal

isr238:
      cli
      push rdi
      mov rdi, 0xee
      jmp isr_normal

isr239:
      cli
      push rdi
      mov rdi, 0xef
      jmp isr_normal

isr240:
      cli
      push rdi
      mov rdi, 0xf0
      jmp isr_normal

isr241:
      cli
      push rdi
      mov rdi, 0xf1
      jmp isr_normal

isr242:
      cli
      push rdi
      mov rdi, 0xf2
      jmp isr_normal

isr243:
      cli
      push rdi
      mov rdi, 0xf3
      jmp isr_normal

isr244:
      cli
      push rdi
      mov rdi, 0xf4
      jmp isr_normal

isr245:
      cli
      push rdi
      mov rdi, 0xf5
      jmp isr_normal

isr246:
      cli
      push rdi
      mov rdi, 0xf6
      jmp isr_normal

isr247:
      cli
      push rdi
      mov rdi, 0xf7
      jmp isr_normal

isr248:
      cli
      push rdi
      mov rdi, 0xf8
      jmp isr_normal

isr249:
      cli
      push rdi
      mov rdi, 0xf9
      jmp isr_normal

isr250:
      cli
      push rdi
      mov rdi, 0xfa
      jmp isr_normal

isr251:
      cli
      push rdi
      mov rdi, 0xfb
      jmp isr_normal

isr252:
      cli
      push rdi
      mov rdi, 0xfc
      jmp isr_normal

isr253:
      cli
      push rdi
      mov rdi, 0xfd
      jmp isr_normal

isr254:
      cli
      push rdi
      mov rdi, 0xfe
      jmp isr_normal

isr255:
      cli
      push rdi
      mov rdi, 0xff
      jmp isr_normal

isr_unsupported:
      cli
      push rdi
      mov rdi, 0
      jmp isr_normal

