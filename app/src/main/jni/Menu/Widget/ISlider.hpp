//
// Created by github.com/seedhollow on 28/10/25.
// Sửa lại theo yêu cầu chọn cách 1
// Đã thêm constructor 5 tham số fix lỗi build
//

#ifndef IL2CPPANDROID_ISLIDER_HPP
#define IL2CPPANDROID_ISLIDER_HPP

#include "BaseWidget.hpp"
#include <algorithm> // Dùng cho std::clamp

class ISlider : public BaseWidget {
public:
    int minValue;
    int maxValue;
    int value; // Thêm biến lưu giá trị hiện tại của thanh trượt

    /* Constructor 4 tham số gốc (Giữ nguyên để không vỡ form cũ) */
    ISlider(int pId, const std::string& pName, int pMin, int pMax)
        : BaseWidget() // Khởi tạo phần lớp cơ sở trước
        , minValue(pMin)
        , maxValue(pMax)
        , value(pMin) // Nếu gọi 4 tham số thì mặc định value = min
    {
        // Đảm bảo min luôn nhỏ hơn hoặc bằng max
        if (minValue > maxValue) {
            std::swap(minValue, maxValue);
        }

        id = pId;
        type = "ISlider";
        name = pName;
    }

    /* Constructor 5 tham số MỚI (Cái mấu chốt để fix lỗi compile) */
    ISlider(int pId, const std::string& pName, int pMin, int pMax, int pInitialValue)
        : BaseWidget()
        , minValue(pMin)
        , maxValue(pMax)
    {
        if (minValue > maxValue) {
            std::swap(minValue, maxValue);
        }
        
        // Gán value và dùng std::clamp để ép giá trị không được vượt quá min/max
        value = std::clamp(pInitialValue, minValue, maxValue);

        id = pId;
        type = "ISlider";
        name = pName;
    }

    // Khai báo lại hàm ToJson rõ ràng, ghi rõ override/final
    json ToJson() const override {
        return {
            {"id",    id},
            {"type",  type},
            {"name",  name},
            {"min",   minValue},
            {"max",   maxValue},
            {"value", value} // Đẩy thêm value sang JSON để UI hiển thị đúng vị trí
        };
    }

    // Đảm bảo có hàm hủy ảo đúng chuẩn kế thừa
    ~ISlider() override = default;
};

#endif //IL2CPPANDROID_ISLIDER_HPP
