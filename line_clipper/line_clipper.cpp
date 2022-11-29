#include <iostream>
#include <vector>
#include <assert.h>

class Point
{
public:
    Point(int x, int y) : x(x), y(y)
    {
    }

    Point(const Point&) = default;
    Point& operator=(const Point& from)
    {
        x = from.x;
        y = from.y;

        return *this;
    }

    friend std::ostream& operator<<(std::ostream& output, const Point& point) {
        output << "(" << point.x << "; " << point.y << ")";

        return output;
    }

    int x;
    int y;
};

class Line
{
public:
    Line(Point start, Point finish) : start(start), finish(finish)
    {
    }

    Line(const Line&) = default;
    Line& operator=(const Line& from)
    {
        auto newStart = from.getStart();
        auto newFinish = from.getFinish();

        start = newStart;
        finish = newFinish;

        return *this;
    }

    const Point getStart() const
    {
        return start;
    }

    const Point getFinish() const
    {
        return finish;
    }

    friend std::ostream& operator<<(std::ostream& output, const Line& line) {
        output << "(" << line.getStart().x << "; " << line.getStart().y << ") - ";
        output << "(" << line.getFinish().x << "; " << line.getFinish().y << ")";

        if (line.IsOutside())
        {
            output << "; is outside";
        }

        return output;
    }

    /*
    line (vector) formula:
    a*i + b*j + c * k = 0

    where:
    a = x2 - x1
    b = y2 - y1
    c = z2 - z1 == zero for two-dimensional lines
    */

    const int getA()
    {
        return finish.x - start.x;
    }

    const int getB()
    {
        return start.y - finish.y;
    }

    const int getC()
    {
        return start.y - finish.y;
    }

    /*
    straight line formula:
    (x - x1) * (x2 - x1) = (y - y1) * (y2 - y1)
    x * (y2 - y1) + y * (x1 - x2) - x1 * y2 + y1 * x2 = 0

    d = (y2 - y1)
    e = (x1 - x2)
    f = y1 * x2 - x1 * y2
    */

    const int getD()
    {
        return finish.y - start.y;
    }

    const int getE()
    {
        return start.x - finish.x;
    }

    const int getF()
    {
        return start.y * finish.x - start.x * finish.y;
    }

    void SetOutside()
    {
        outside = true;
    }

    bool IsOutside() const
    {
        return outside;
    }
private:
    Point start;
    Point finish;
    bool outside = false;
};


class LineClipper
{
public:
    LineClipper(Point leftUpper, Point rightLower)
        : vpLeft(Point(leftUpper.x, leftUpper.y), Point(leftUpper.x, rightLower.y)),
        vpRight(Point(rightLower.x, leftUpper.y), Point(rightLower.x, rightLower.y)),
        vpTop(Point(leftUpper.x, leftUpper.y), Point(rightLower.x, leftUpper.y)),
        vpBottom(Point(leftUpper.x, rightLower.y), Point(rightLower.x, rightLower.y))
    {
    }

    Line Clip(Line line)
    {
        if (IsOutside(line))
        {
            line.SetOutside();
        }
        else if (!IsInside(line))
        {
            auto newStart = Point(0, 0);

            Line viewportLines[4] = { vpLeft, vpRight, vpTop, vpBottom };
            auto intersectionPoints = new std::vector<Point>();

            for (auto i = 0; i < 4; i++)
            {
                std::cout << "Has intersection with " << viewportLines[i];
                if (HasIntersection(line, viewportLines[i]))
                {
                    std::cout << " yes";
                    std::cout << std::endl;
                    intersectionPoints->push_back(
                        GetIntersectionPoint(line, viewportLines[i])
                    );
                }
                else
                {
                    std::cout << " no";
                    std::cout << std::endl;
                }

                if (intersectionPoints->size() == 2)
                {
                    break;
                }
            }

            assert(intersectionPoints->size() != 0);


            if (intersectionPoints->size() == 2)
            {
                auto newLine = Line(intersectionPoints->at(0), intersectionPoints->at(1));
                line = newLine;
            }
            else
            {
                auto newStart = Point(0, 0);

                if (isPointInside(line.getStart()))
                {
                    newStart = line.getStart();
                }
                else
                {
                    newStart = line.getFinish();
                }

                auto newLine = Line(newStart, intersectionPoints->at(0));
                line = newLine;
            }
        }

        return line;
    }

private:
    Line vpLeft;
    Line vpRight;
    Line vpTop;
    Line vpBottom;

    // is line (start, finish) outside of viewport coordinates (left-upper, right-bottom)?
    bool IsOutside(Line line)
    {
        auto isOutside = false;
        auto minX = vpTop.getStart().x;
        auto maxX = vpTop.getFinish().x;

        if ((line.getStart().x <= minX && line.getFinish().x <= minX)
            || (line.getStart().x >= maxX && line.getFinish().x >= maxX))
        {
            isOutside = true;
        }

        if (!isOutside)
        {
            auto minY = vpTop.getStart().y;
            auto maxY = vpBottom.getStart().y;

            if ((line.getStart().y <= minY && line.getFinish().y <= minY)
                || (line.getStart().y >= maxY && line.getFinish().y >= maxY))
            {
                isOutside = true;
            }
        }

        return isOutside;
    }

    bool isPointInside(const Point point)
    {
        auto isInside = false;
        auto minX = vpTop.getStart().x;
        auto maxX = vpTop.getFinish().x;
        auto minY = vpTop.getStart().y;
        auto maxY = vpBottom.getStart().y;

        if (point.x > minX && point.x < maxX
            && point.y > minY && point.y < maxY)
        {
            isInside = true;
        }

        return isInside;
    }

    // is lines inside of viewport coordinates?
    bool IsInside(const Line line)
    {
        auto isInside = false;
        auto minX = vpTop.getStart().x;
        auto maxX = vpTop.getFinish().x;
        auto minY = vpTop.getStart().y;
        auto maxY = vpBottom.getStart().y;

        const Point startPoint = line.getStart();

        if (isPointInside(startPoint)
            && isPointInside(line.getFinish())
            )
        {
            isInside = true;
        }

        return isInside;
    }

    // does intersect:
    bool HasIntersection(Line first, Line second)
    {
        // https://habr.com/ru/post/267037/

        auto firstToStartSection = Line(first.getStart(), second.getStart());
        auto firstToFinishSection = Line(first.getStart(), second.getFinish());

        auto firstToStartZ = (first.getA() * firstToStartSection.getB() - first.getB() * firstToStartSection.getA());
        auto firstToFinishZ = (first.getA() * firstToFinishSection.getB() - first.getB() * firstToFinishSection.getA());

        auto doIntersect = (firstToStartZ < 0) ? (firstToFinishZ >= 0) : (firstToFinishZ < 0); // true if signs are opposite

        if (doIntersect) // if true from one vector, check also from another. true if both are true
        {
            auto secondToStartSection = Line(second.getStart(), first.getStart());
            auto secondToFinishSection = Line(second.getStart(), first.getFinish());

            auto secondToStartZ = (second.getA() * secondToStartSection.getB() - second.getB() * secondToStartSection.getA());
            auto secondToFinishZ = (second.getA() * secondToFinishSection.getB() - second.getB() * secondToFinishSection.getA());

            doIntersect = (secondToStartZ < 0) ? (secondToFinishZ >= 0) : (secondToFinishZ < 0); // true if signs are opposite
        }

        return doIntersect;
    }

    //  find intersection points = new line
    Point GetIntersectionPoint(Line first, Line second)
    {
        // https://www.interestprograms.ru/source-codes-tochka-peresecheniya-dvuh-pryamyh-na-ploskosti
        // x = (e1 * f2 - e2 * f1) / (d1 * e2 - d2 * e1)
        // y = (d2 * f1 - d1 * f2) / (d1 * e2 - d2 * e1)

        int d1 = first.getD();
        int e1 = first.getE();
        int f1 = first.getF();

        int d2 = second.getD();
        int e2 = second.getE();
        int f2 = second.getF();

        auto result = Point(
            (e1 * f2 - e2 * f1) / (d1 * e2 - d2 * e1),
            (d2 * f1 - d1 * f2) / (d1 * e2 - d2 * e1)
        );

        std::cout << "intersection at " << result << std::endl;

        return result;
    }
};

int main()
{
    auto firstLine = Line(Point(8, 7), Point(8, 13));
    auto L1 = Line(Point(10, 9), Point(15, 11));
    auto L2 = Line(Point(6, 9), Point(12, 15));
    auto L3 = Line(Point(15, 5), Point(21, 3));
    auto L4 = Line(Point(17, 12), Point(22, 11));

    auto clipper = LineClipper(Point(8, 7), Point(19, 13));

    /*std::cout << "   L1: " << L1 << std::endl;
    auto newL1 = clipper.Clip(L1);
    std::cout << "newL1: " << newL1 << std::endl;

    std::cout << "   L2: " << L2 << std::endl;
    auto newL2 = clipper.Clip(L2);
    std::cout << "newL2: " << newL2 << std::endl;

    std::cout << "   L3: " << L3 << std::endl;
    auto newL3 = clipper.Clip(L3);
    std::cout << "newL3: " << newL3 << std::endl;*/

    std::cout << "   L4: " << L4 << std::endl;
    auto newL4 = clipper.Clip(L4);
    std::cout << "newL4: " << newL4 << std::endl;

    /*auto intersectionPoint = GetIntersectionPoint(firstLine, L1);

    std::cout << "intersection point x: " << intersectionPoint.x << "; y: " << intersectionPoint.y << std::endl;*/

    //std::cout << "ranges 8 - 19 AND 10 - 15" << std::endl;

    /*if (DoesRangesIntersect(8, 19, 15, 21))
    {
        std::cout << "intersect" << std::endl;
    }
    else
    {
        std::cout << "do not intersect" << std::endl;
    }*/
}
