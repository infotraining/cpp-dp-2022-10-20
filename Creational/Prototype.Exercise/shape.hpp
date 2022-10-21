#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "point.hpp"

namespace Drawing
{
    class Shape
    {
    public:
        virtual ~Shape() = default;
        virtual void move(int x, int y) = 0;
        virtual void draw() const = 0;
        virtual std::unique_ptr<Shape> clone() const = 0;
    };

    template<typename TDerived, typename TBase = Shape>
    class CloneableShape : public TBase
    {
        std::unique_ptr<Shape> clone() const override
        {
            return std::make_unique<TDerived>(static_cast<const TDerived&>(*this));
        }
    };

    template <typename TShape>
    class ShapeBase : public CloneableShape<TShape>
    {
        Point coord_; // composition
    public:
        Point coord() const
        {
            return coord_;
        }

        void set_coord(const Point& pt)
        {
            coord_ = pt;
        }

        ShapeBase(int x = 0, int y = 0)
            : coord_{x, y}
        {
        }

        void move(int dx, int dy) override
        {
            coord_.translate(dx, dy);
        }
    };
}

#endif // SHAPE_HPP
