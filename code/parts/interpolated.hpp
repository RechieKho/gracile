#ifndef INTERPOLATED_HPP
#define INTERPOLATED_HPP

#include <concepts>
#include <cmath>

#include "definition.hpp"
#include "part.hpp"

template <class TType>
concept IsInterpolatable = std::integral<TType> || std::floating_point<TType>;

template <IsInterpolatable TType>
class Interpolated final : public Part<>
{
public:
    using ValueType = TType;
    using WeightType = ::FloatType;

private:
    ValueType current;

public:
    ValueType target;
    ValueType minSignificantDifference;
    WeightType incrementWeight;
    WeightType decrementWeight;

    Interpolated(
        ValueType pValue,
        ValueType pMinSignificantDifrerence,
        WeightType pIncrementWeight = 0.8f,
        WeightType pDecrementWeight = 0.8f)
        : current(pValue),
          target(pValue),
          minSignificantDifference(pMinSignificantDifrerence),
          incrementWeight(pIncrementWeight),
          decrementWeight(pDecrementWeight) {}
    ~Interpolated() override = default;

    auto Interpolate(WeightType pFactor = 1.0f) const -> ValueType
    {
        const auto difference = (target - current);
        if (std::abs(difference) < minSignificantDifference)
            return current;
        return current + difference * (difference < 0 ? decrementWeight : incrementWeight) * pFactor;
    }

    auto IsDifferenceSignificant() const -> BoolType { return std::abs(target - current) < minSignificantDifference; }
    auto Difference() const -> ValueType { return (target - current); }
    auto ViewCurrent() const -> const ValueType & { return current; }

    auto Process() -> void override
    {
        current = Interpolate();
    }
};

#endif // INTERPOLATED_HPP