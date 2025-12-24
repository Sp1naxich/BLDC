# BLDC集成控制板
根据原理图，基础使用ODrive单电机方案，省略CAN接口、Hall接口
## 硬件设计
- 基于STM32F405RGT6微控制器
- 采用IR2010S控制芯片，使用PWM互补输出
- 低端双电阻采样，采样电阻0.01Ω/1%，支持正负方向电流，参考电压1.65V
- 母线电压采样
- 1个复位按钮，4个功能按钮，其中按钮3使用WAKE0上拉
## 软件架构
- STM32CubeMX生成初始代码
- 无OS系统
# 电路设计
## 片上IO引出
<img width="690" height="475" alt="image" src="https://github.com/user-attachments/assets/00097066-4113-4196-8451-939c9f620546" />
## 硬件实物
<img width="643" height="346" alt="image" src="https://github.com/user-attachments/assets/948858fb-b480-4aa2-b9e1-37562be504c3" />
## 母线电压采样
<img width="210" height="346" alt="image" src="https://github.com/user-attachments/assets/c9e741a0-e5c5-44d8-a421-0f1967943e24" />
## 低端相电流采样
<img width="544" height="476" alt="image" src="https://github.com/user-attachments/assets/ffc32b26-0b78-46c2-91d2-c20465ed0da5" />
<img width="614" height="593" alt="image" src="https://github.com/user-attachments/assets/bec8abd9-f2a2-4a28-8fcf-cbc06c2a913c" />
## 1.65V参考电压
<img width="429" height="252" alt="image" src="https://github.com/user-attachments/assets/d08f3d38-4689-40d5-b19f-2a3db65335bc" />
## IR2010S电路
<img width="687" height="303" alt="image" src="https://github.com/user-attachments/assets/efba93a0-cf54-4342-8e2f-3cf175b0c6c3" />
## 半桥电路
<img width="364" height="713" alt="image" src="https://github.com/user-attachments/assets/2442b19d-225d-4c64-947e-05f3430efcfb" />
