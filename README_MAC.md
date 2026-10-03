# 🥷 Hướng Dẫn Build iOS & Upload TestFlight - Ninja World

Dự án này được thiết kế để biên dịch trực tiếp trên **macOS bằng Xcode**, sử dụng nền tảng **Cocos2d-x 2.2.6 (Hỗ trợ 64-bit arm64 cho iPhone hiện đại)** và nạp trực tiếp toàn bộ **741 file `.ccbi`** gốc của game.

---

## 📁 Cấu trúc thư mục dự án

```
NinjaWorld-iOS/
├── Classes/                     # Mã nguồn C++ khởi tạo game & nạp CCBI
│   ├── AppDelegate.h / .cpp     # Thiết lập khung hình 768x960, nạp LoginView.ccbi
│   └── CCBManager.h / .cpp      # Wrapper của CCBReader nạp giao diện chuẩn gốc
├── proj.ios_mac/                # Dự án iOS cho Xcode
│   ├── NinjaWorld.xcodeproj     # Project Xcode
│   ├── ios/                     # AppController, RootViewController, Info.plist
│   └── Prefix.pch
├── Resources/                   # Toàn bộ tài nguyên gốc (5.204 tệp):
│   ├── ccbResources/            # 741 file .ccbi & sprite sheets gốc
│   ├── script/                  # 382 file Lua logic gameplay gốc
│   └── baseconfig.json          # Cấu hình IP máy chủ (160.22.123.62:8088)
├── setup_mac.sh                 # Script tự động tải cocos & mở Xcode 1-click
└── README_MAC.md
```

---

## 🚀 CÁCH 1: Chạy tự động 1 lệnh (Khuyên dùng - Nhanh nhất)

Chỉ cần mở Terminal trên Mac tại thư mục này và gõ:

```bash
bash setup_mac.sh
```

Script này sẽ tự động:
1. Tải bộ engine Cocos2d-x 2.2.6 (đã hỗ trợ 64-bit arm64).
2. Thiết lập dự án Xcode chuẩn và tích hợp sẵn toàn bộ 5.204 tài nguyên gốc cùng mã nguồn C++.
3. Tự động mở Xcode lên màn hình.

---

## 🛠️ CÁCH 2: Cấu hình thủ công

### Bước 1: Chuẩn bị Cocos2d-x 2.2.6 Engine

Nếu trên máy Mac chưa có thư viện Cocos2d-x 2.2.6, mở Terminal và clone nhánh 2.2.6 về máy:
```bash
git clone https://github.com/cocos2d/cocos2d-x.git --branch cocos2d-x-2.2.6 --depth 1
```

### Bước 2: Mở dự án trong Xcode

1. Mở file **`proj.ios_mac/NinjaWorld.xcodeproj`**.
2. Chọn target **`NinjaWorld`**.
3. Vào tab **Signing & Capabilities**:
   * Tích chọn **Automatically manage signing**.
   * Chọn **Team** (Tài khoản Apple Developer đã có quyền upload TestFlight).
   * Bundle Identifier: `com.ninja.world` (hoặc đổi theo Bundle ID trong tài khoản Developer).

---

## 📲 Build & Upload lên TestFlight

### Kiểm tra trên iPhone thật
1. Cắm iPhone vào máy Mac qua cáp USB.
2. Trên thanh công cụ Xcode, chọn thiết bị đích là chiếc **iPhone của bạn**.
3. Bấm tổ hợp phím **`Cmd + R`** (hoặc nút Play ▶️) để build và cài trực tiếp lên iPhone.
4. Game sẽ khởi động, bung toàn màn hình và load ngay lập tức `LoginView.ccbi` chuẩn gốc.

### Đóng gói và Upload lên TestFlight
1. Chọn thiết bị đích là **Any iOS Device (arm64)**.
2. Trên menu trên cùng: Chọn **Product → Archive**.
3. Khi quá trình biên dịch hoàn tất, cửa sổ **Organizer** sẽ hiện ra:
   * Bấm **Distribute App**.
   * Chọn **App Store Connect** (hoặc **TestFlight Internal/External**).
   * Bấm **Next** để Xcode tự động tải bản build lên Apple TestFlight.
4. Sau khoảng 5–10 phút, người chơi trên iPhone chỉ cần mở ứng dụng **TestFlight** là có thể tải game về trải nghiệm!
