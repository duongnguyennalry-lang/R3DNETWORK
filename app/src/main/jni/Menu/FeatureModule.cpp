//
// ThrowIO Mod — R3DNETWORK Menu
//

#include "FeatureModule.hpp"
#include "../Hacks/Vars.h"
#include "../Hacks/Hooks.hpp"
#include "Includes/Logger.h"
#include "Includes/obfuscate.h"


void FeatureModule::OnDraw(JNIEnv *env, jclass clazz,
                           jobject draw_view, jobject canvas) {
    // Nếu sau này cần ESP / visual drawing thì thêm vào đây
    // Draw draw(env, draw_view, canvas);
    // if (draw.isValid()) Visuals::Update(draw, draw.getWidth(), draw.getHeight());
}


jstring FeatureModule::GetFeatureList(JNIEnv *env, jobject context) {
    RegisterFeatures();
    std::string json = Widget::ToJsonString();
    return env->NewStringUTF(json.c_str());
}


// ================================================================
//  ⭐ MENU DEFINITION — ID PHẢI UNIQUE CHO MỖI WIDGET ⭐
// ================================================================
void FeatureModule::RegisterFeatures() {

    // ── HEADER ────────────────────────────────────────────────
    Widget::Add(ITextView(OBFUSCATE(
        "<font color=#00BFFF>THROWIO MOD</font> — "
        "<font color=#00FF00>AXIOM DEVELOPMENT</font>\n"
        "<font color=#888888>Hardcoded offsets edition</font>")));

    // ── NHÓM: TIỀN TỆ ────────────────────────────────────────
    Widget::Add(ICategory(OBFUSCATE("💵 TIỀN TỆ")));

    Widget::Add(ISwitch(
        /* ID */      1,
        /* Tên */     OBFUSCATE("Tiền mềm vô hạn"),
        /* Mô tả */   OBFUSCATE("Soft money luôn ở mức tối đa"),
        /* Giá trị */ Vars::PlayerBalance::infiniteMoney.load()));

    Widget::Add(ISwitch(
        /* ID */      2,
        /* Tên */     OBFUSCATE("Tiền premium vô hạn"),
        /* Mô tả */   OBFUSCATE("Hard money / kim cương luôn tối đa"),
        /* Giá trị */ Vars::PlayerBalance::infinitePremium.load()));

    Widget::Add(ISwitch(
        /* ID */      3,
        /* Tên */     OBFUSCATE("Bỏ quảng cáo"),
        /* Mô tả */   OBFUSCATE("Tắt toàn bộ quảng cáo trong game"),
        /* Giá trị */ Vars::PlayerBalance::noAds.load()));

    Widget::Add(IButtonLink(
        /* ID */      4,
        /* Tên */     OBFUSCATE("⚡ Cập nhật tiền ngay"),
        /* Action */  OBFUSCATE("action:force_money")));

    // ── NHÓM: TIẾN TRÌNH ─────────────────────────────────────
    Widget::Add(ICategory(OBFUSCATE("📈 TIẾN TRÌNH")));

    Widget::Add(ISwitch(
        /* ID */      5,
        /* Tên */     OBFUSCATE("Max level 99"),
        /* Mô tả */   OBFUSCATE("Luôn giữ level ở mức tối đa"),
        /* Giá trị */ Vars::PlayerBalance::maxLevel.load()));

    Widget::Add(IButtonLink(
        /* ID */      6,
        /* Tên */     OBFUSCATE("⚡ Lên cấp ngay"),
        /* Action */  OBFUSCATE("action:force_level")));

    // ── NHÓM: CHIẾN ĐẤU ──────────────────────────────────────
    Widget::Add(ICategory(OBFUSCATE("⚔️ CHIẾN ĐẤU")));

    Widget::Add(ISwitch(
        /* ID */      7,
        /* Tên */     OBFUSCATE("God Mode (Bất tử)"),
        /* Mô tả */   OBFUSCATE("Không nhận sát thương từ bất kỳ nguồn nào"),
        /* Giá trị */ Vars::Combat::godMode.load()));

    Widget::Add(ISwitch(
        /* ID */      8,
        /* Tên */     OBFUSCATE("Speed Hack"),
        /* Mô tả */   OBFUSCATE("Tăng tốc độ tấn công / di chuyển"),
        /* Giá trị */ Vars::Combat::speedHack.load()));

    // Speed multiplier slider bên trong Collapse
    Widget::Add(ICollapse(OBFUSCATE("⚙️ Cài đặt tốc độ"), {
        ICollapse::Child(ISlider(
            /* ID */     9,
            /* Tên */    OBFUSCATE("Hệ số tốc độ"),
            /* Min */    10,   // x1.0
            /* Max */    50,   // x5.0
            /* Giá trị */static_cast<int>(
                Vars::Combat::speedMultiplier.load(std::memory_order_relaxed) * 10.0f)))
    }));

    // ── NHÓM: BẢO MẬT ────────────────────────────────────────
    Widget::Add(ICategory(OBFUSCATE("🛡️ BẢO MẬT")));

    Widget::Add(ISwitch(
        /* ID */      10,
        /* Tên */     OBFUSCATE("Bypass Anti-Cheat"),
        /* Mô tả */   OBFUSCATE("Watchdog liên tục cập nhật tránh phát hiện"),
        /* Giá trị */ Vars::Security::antiCheat.load()));

    // ── FOOTER ────────────────────────────────────────────────
    Widget::Add(ICategory(OBFUSCATE("ℹ️ THÔNG TIN")));
    Widget::Add(ITextView(OBFUSCATE(
        "<font color=#888888>Framework: </font>"
        "<font color=#FFA500>R3DNETWORK (LGL)</font>\n"
        "<font color=#888888>Offset type: </font>"
        "<font color=#00FF00>Hardcoded</font>")));
}


// ================================================================
//  ⭐ EVENT HANDLER — KHI NGƯỜI DÙNG BẤT/TẮT TOGGLE ⭐
//  featNum = ID của widget đã khai báo ở trên
// ================================================================
void FeatureModule::OnFeatureChanged(JNIEnv *env, jclass clazz, jobject obj,
                                     jint featNum, jstring featName,
                                     jint value, jboolean boolean,
                                     jstring str) {
    const char* name = env->GetStringUTFChars(featName, nullptr);
    const char* strVal = (str != nullptr)
        ? env->GetStringUTFChars(str, nullptr)
        : "";

    LOGD(OBFUSCATE("Feature changed: [%d] %s | val=%d | bool=%d | str=%s"),
         featNum, name, value, (int)boolean, strVal);

    switch (featNum) {

        // ── TIỀN TỆ ──────────────────────────────────────────
        case 1:
            Vars::PlayerBalance::infiniteMoney.store((bool)boolean);
            break;

        case 2:
            Vars::PlayerBalance::infinitePremium.store((bool)boolean);
            break;

        case 3:
            Vars::PlayerBalance::noAds.store((bool)boolean);
            break;

        case 4:
            // Button: Cập nhật tiền ngay
            Vars::PlayerBalance::forceUpdateMoney.store(true);
            Hooks::ApplyWatchdog(); // Gọi ngay lập tức
            break;

        // ── TIẾN TRÌNH ───────────────────────────────────────
        case 5:
            Vars::PlayerBalance::maxLevel.store((bool)boolean);
            break;

        case 6:
            // Button: Lên cấp ngay
            Vars::PlayerBalance::forceMaxLevel.store(true);
            Hooks::ApplyWatchdog(); // Gọi ngay lập tức
            break;

        // ── CHIẾN ĐẤU ────────────────────────────────────────
        case 7:
            Vars::Combat::godMode.store((bool)boolean);
            break;

        case 8:
            Vars::Combat::speedHack.store((bool)boolean);
            break;

        case 9:
            // Slider: value = 10-50 → chia 10 ra x1.0 - x5.0
            Vars::Combat::speedMultiplier.store(
                static_cast<float>(value) / 10.0f,
                std::memory_order_relaxed);
            break;

        // ── BẢO MẬT ──────────────────────────────────────────
        case 10:
            Vars::Security::antiCheat.store((bool)boolean);
            break;

        default:
            LOGD(OBFUSCATE("Unhandled feature ID: %d"), featNum);
            break;
    }

    env->ReleaseStringUTFChars(featName, name);
    if (str != nullptr) env->ReleaseStringUTFChars(str, strVal);
}
