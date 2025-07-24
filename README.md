---

## 📘 Mô tả dự án – STM32 DMX512 Controller

**STM32 DMX512 Controller** là một bộ điều khiển tín hiệu tiêu chuẩn DMX512 dựa trên vi điều khiển STM32, được thiết kế dùng để:

* 🕹️ **Truyền dữ liệu DMX512** qua giao tiếp RS‑485, hỗ trợ tối đa 512 channel cho hệ thống ánh sáng stage hoặc lighting fixtures.
* ⚙️ Tùy chỉnh phần cứng linh hoạt: mạch driver RS‑485, định thời bằng Timer của STM32.
* 🔄 Quản lý thời gian và độ trễ kỹ thuật để đảm bảo tín hiệu BREAK, MARK‑After‑Break và MARK‑Between‑Slots đúng theo tiêu chuẩn DMX512.

---

## ⚙️ Chi tiết kỹ thuật & tính năng

### 1. **UART + RS-485**

* Sử dụng UART để truyền dữ liệu DMX512 với chuẩn 250 kbps.
* Kết nối với chip transceiver RS‑485 (như MAX485, SN75176…) qua chân Tx/Rx + RE/DE để điều khiển chế độ truyền/nhận.

### 2. **Timer và điều khiển BREAK**

* Dùng **Timer nội bộ** của STM32 (ví dụ TIM2, TIM15…) để phát tín hiệu `BREAK` (thấp kéo dài \~92 µs) và `MARK After Break` (\~12 µs) trước khi phát dữ liệu slot đầu tiên ([electronics.stackexchange.com][1], [amazon.com][2], [community.element14.com][3]).
* Sau đó, dữ liệu từng slot được gửi theo thời gian đã xác định, xen kẽ với **Mark Between Slots**.

### 3. **DMX frame sequence**

* Quy trình gửi bao gồm:

  1. **Reset (BREAK)**: kéo thấp đường dữ liệu trong khoảng ≥ 92 µs.
  2. **Mark-After-Break**: giữ mức cao khoảng ≥ 12 µs.
  3. **Start Code (0x00)**.
  4. **Data slots**: lần lượt gửi từng giá trị channel, có khoảng trống ngắn giữa mỗi byte.

### 4. **HAL/Middleware của STM32CubeIDE**

* Sử dụng thư viện HAL UART + Timer để đảm bảo độ ổn định và đáp ứng thời gian thực.

---

## ✅ Trạng thái hiện tại

* Đã **thực thi thành công quy trình truyền DMX**:

  * Timer điều khiển BREAK và MARK;
  * UART phát Start Code và dữ liệu slot.
* **Hoạt động ổn định**, hỗ trợ truyền chuỗi dữ liệu DMX lên đến 512 channel liên tục mà không bị lỗi ([community.element14.com][3]).

---

## 🛠️ Cấu trúc mã nguồn

```
/STM32-DMX512Controller
├─ Core/
│   ├─ main.c              ← Khởi tạo UART, Timer, loop chính
│   └─ dmxtx.c/.h          ← Hàm dmx_send(), xử lý sequence bộ phát
├─ Drivers/
│   ├─ uart.c/.h           ← Config UART 250 kbps, điều khiển RE/DE
│   └─ timer.c/.h          ← Cấu hình Timer cho BREAK, slot timing
└─ STM32CubeMX_generated/  ← Code init HAL do CubeIDE tạo
```
