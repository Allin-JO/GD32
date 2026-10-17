# 通信协议

## 1. 运动控制板

USART1（ESP8266）或 USART2（蓝牙/备用），9600-8-N-1。

### 运动指令

```json
{"command":"forward"}
{"command":"backward"}
{"command":"left"}
{"command":"right"}
{"command":"stop"}
{"command":"forward","speed":2000}
{"speed":20}
```

`speed` 单位 mm/s，范围 0～3000。

### ACK

```text
OK:forward
OK:backward
OK:left
OK:right
OK:stop
SPD:2000
ERR:unknown command
ERR:json format error
```

兼容单数字指令：`0` 停止、`1` 前进、`2` 左转、`3` 右转、`4` 后退。

## 2. 喷洒控制板

USART2，9600-8-N-1。

```json
{"control":1}
{"control":0}
{"tare":1}
{"weight":1234}
```

- `control=1`：舵机到工作位，开启雾化，进入周期补液。
- `control=0`：关闭雾化/水泵，舵机回待机位。
- `tare=1`：以当前重量为零点去皮，并写入 Flash。
- `weight`：默认每 3 s 上报一次，单位 g。

## 3. MQTT 主题

| 主题 | 方向 | 内容 |
|---|---|---|
| `spray/weight` | 设备 → 云服务 | 消毒液重量 |
| `spray/control` | 云服务 → 设备 | 控制/去皮 |
| `device/wdlmj/control` | 云服务 → 运动板 | 运动指令 |
