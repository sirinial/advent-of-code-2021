#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#define VERBOSE(X)
#define VERBOSEa(X)                                                                                                                                                                                    \
    {                                                                                                                                                                                                  \
        std::cout << X << std::endl;                                                                                                                                                                   \
    }

struct ReductionInfo
{
    bool tryToExplode{true};
    bool tryToSplit{false};

    bool success = false;

    int* lastElement = nullptr;
    int* nextElement = nullptr;

    int add2Right{0};
    int add2Left{0};
};

using ElementPtr = std::shared_ptr<const class Element>;

std::ostream& operator<<(std::ostream& o, const ElementPtr& el);
std::ostream& operator<<(std::ostream& o, const class Element& el);

struct Element
{
    virtual ElementPtr oneReductionRecursive(ReductionInfo& info, const int level = 4) const = 0;

    std::pair<bool, ElementPtr> oneReduction() const
    {
        {
            ReductionInfo info;

            ElementPtr result = oneReductionRecursive(info);

            if(info.success)
            {
                if(info.lastElement != nullptr)
                {
                    *info.lastElement += info.add2Left;
                }
                if(info.nextElement != nullptr)
                {
                    *info.nextElement += info.add2Right;
                }

                VERBOSE("  - exploded " << (*this));
                return {true, result};
            }
        }
        {
            ReductionInfo info;
            info.tryToExplode = false;
            info.tryToSplit   = true;

            ElementPtr result = oneReductionRecursive(info);

            if(info.success)
            {
                VERBOSE("  - splitted " << (*this));
                return {true, result};
            }
        }

        return {false, this->copy()};
    }

    ElementPtr reducted() const
    {

        VERBOSE("  - Reducing " << (*this));
        std::pair<bool, ElementPtr> r = oneReduction();

        while(r.first)
        {
            r = r.second->oneReduction();
        }

        return r.second;
    }

    virtual int        magnitude() const                                 = 0;
    virtual ElementPtr copy() const                                      = 0;
    virtual void       print(std::ostream& o, const int level = 1) const = 0;

    virtual bool isDataElement() const
    {
        return false;
    }

    virtual ~Element() = default;
};

struct DataElement : public Element
{
    static ElementPtr createElement(const int a)
    {
        return std::make_shared<DataElement>(a);
    }

    ElementPtr copy() const
    {
        return createElement(el);
    }

    DataElement(int a)
        : el(a)
    {}

    bool isDataElement() const
    {
        return true;
    }

    void print(std::ostream& o, const int) const
    {
        o << el;
    }
    int el;

    virtual int magnitude() const
    {
        return el;
    }

    ElementPtr oneReductionRecursive(ReductionInfo& info, const int level) const
    {
        if(info.tryToSplit && info.success == false)
        {
            if(el > 9)
            {
                info.success = true;

                VERBOSE("      --> Splitting " << *this);
                return split();
            }
        }
        std::shared_ptr<DataElement> r = std::make_shared<DataElement>(el);

        if(info.success == false)
        {
            info.lastElement = &r->el;
        }
        else if(info.nextElement == nullptr)
        {
            info.nextElement = &r->el;
        }

        return r;
    };

    ElementPtr split() const;
};

struct PairElement : public Element
{
    static ElementPtr createElement(ElementPtr a, ElementPtr b)
    {
        return std::make_shared<PairElement>(a, b);
    }

    ElementPtr copy() const
    {
        return createElement(lhs, rhs);
    }

    PairElement(ElementPtr a, ElementPtr b)
        : lhs(a)
        , rhs(b)
    {}

    void print(std::ostream& o, const int level) const
    {
        o << "[";
        lhs->print(o, level + 1);
        o << ",";
        rhs->print(o, level + 1);
        o << "]";
    }

    ElementPtr oneReductionRecursive(ReductionInfo& info, const int level) const
    {
        if(info.tryToExplode && info.success == false && level <= 0)
        {
            if(lhs->isDataElement() && rhs->isDataElement())
            {
                info.success   = true;
                info.add2Right = rhs->magnitude();
                info.add2Left  = lhs->magnitude();

                VERBOSE("      --> Exploding" << *this);

                return DataElement::createElement(0);
            }
        }
        ElementPtr newLhs = lhs->oneReductionRecursive(info, level - 1);
        ElementPtr newRhs = rhs->oneReductionRecursive(info, level - 1);

        return createElement(newLhs, newRhs);
    };

    virtual int magnitude() const
    {
        return 3 * lhs->magnitude() + 2 * rhs->magnitude();
    }

    ElementPtr lhs, rhs;
};

ElementPtr DataElement::split() const
{
    int x = el / 2;

    return PairElement::createElement(DataElement::createElement(x), DataElement::createElement(el - x));
}

ElementPtr add(const ElementPtr& lhs, const ElementPtr& rhs)
{
    return PairElement::createElement(lhs, rhs)->reducted();
}

ElementPtr readElement(std::istream& i)
{
    if(i.peek() == '[')
    {
        i.get();
        ElementPtr lhs = readElement(i);
        i.get();
        ElementPtr rhs = readElement(i);
        i.get();
        return PairElement::createElement(lhs, rhs);
    }
    int a;
    i >> a;
    return DataElement::createElement(a);
}

std::ostream& operator<<(std::ostream& o, const ElementPtr& el)
{
    el->print(o);
    return o;
}

std::ostream& operator<<(std::ostream& o, const Element& el)
{
    el.print(o);
    return o;
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cerr << "Missing argument!" << std::endl;
        return 1;
    }
    const std::string fileName = argv[1];

    std::ifstream infile(fileName);
    std::cout << "Reading " << fileName << std::endl;

    ElementPtr total;

    while(infile.peek() != EOF)
    {
        ElementPtr el = readElement(infile);

        if(total)
        {
            std::cout << "==============================" << std::endl;
            std::cout << "   " << total << std::endl;
            std::cout << " + " << el << std::endl;

            total = add(total, el);
            std::cout << "-------------------------------------------" << std::endl;

            std::cout << "   " << total << std::endl;
            std::cout << "==============================" << std::endl;
        }
        else
        {
            total = el;
        }
        if(infile.peek() == '\n')
        {
            infile.get();
        }
    }

    std::cout << "---> " << total->magnitude() << std::endl;

    return 0;
}
