#include <cstdint>
#include <ratio>

namespace usu
{
    template <class R, class S = std::uint64_t>
    class weight
    {
      private:
        S weightCount;

      public:
        constexpr static R ratio;
        constexpr static S storageType = 0;
        weight() :
            weightCount(0) {}
        weight(const S& count) :
            weightCount(count) {}
        S count() const { return weightCount; }
        weight<R, S> operator+(const weight<R, S>& add) const
        {
            const S newCount = weightCount + add.count();
            const weight<R, S> newweight(newCount);
            return newweight;
        }
        weight<R, S> operator-(const weight<R, S>& subtract) const
        {
            const S newCount = weightCount - subtract.count();
            const weight<R, S> newweight(newCount);
            return newweight;
        }
    };

    using microgram = weight<std::micro>;
    using gram = weight<std::ratio<1, 1>>;
    using kilogram = weight<std::ratio<1000, 1>>;
    using ounce = weight<std::ratio<2834952, 100000>, double>;
    using pound = weight<std::ratio<4535924, 10000>, double>;
    using ton = weight<std::ratio<9071847, 10>, double>;

    template <class W>
    W operator*(const W& weight, const double& scalar)
    {
        const auto count = weight.count();
        const auto newCount = static_cast<decltype(count)>(scalar * count);
        const W newweight(newCount);
        return newweight;
    }

    template <class W>
    W operator*(const double& scalar, const W& weight)
    {
        const auto count = weight.count();
        const auto newCount = static_cast<decltype(count)>(scalar * count);
        const W newweight(newCount);
        return newweight;
    }

    template <class NW, class OW>
    NW weight_cast(const OW& weight)
    {
        auto nn = NW::ratio.num;
        auto nd = NW::ratio.den;
        auto on = OW::ratio.num;
        auto od = OW::ratio.den;
        const auto newCount = static_cast<decltype(NW::storageType)>(static_cast<double>(weight.count() * on * nd) / (nn * od));
        NW newweight(newCount);
        return newweight;
    }
}; // namespace usu
