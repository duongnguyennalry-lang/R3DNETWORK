//
// Created by github.com/seedhollow on 28/10/25.
// Sửa lại theo yêu cầu chọn cách 1
//

#ifndef IL2CPPANDROID_ISLIDER_HPP
#define IL2CPPANDROID_ISLIDER_HPP

#include "BaseWidget.hpp"
#include <algorithm> // Dùng cho std::clamp nếu cần kiểm tra giá trị

class ISlider : public BaseWidget {
public:
    int minValue;
    int maxValue;

    /* Constructor
     * pId: Numeric ID of the slider
     * pName: Name of the slider
     * pMin: Minimum value of the slider
     * pMax: Maximum value of the slider
     */
    ISlider(int pId, const std::string& pName, int pMin, int pMax)
        : BaseWidget() // Khởi tạo phần lớp cơ sở trước
        , minValue(pMin)
        , maxValue(pMax)
    {
        // Đảm bảo min luôn nhỏ hơn hoặc bằng max, tự điều chỉnh nếu nhập sai
        if (minValue > maxValue) {
            std::swap(minValue, maxValue);
        }

        // Gán giá trị cho thành phần của lớp cơ sở
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
            {"max",   maxValue}
        };
    }

    // Đảm bảo có hàm hủy ảo đúng chuẩn kế thừa
    ~ISlider() override = default;
};

#endif //IL2CPPANDROID_ISLIDER_HPP
