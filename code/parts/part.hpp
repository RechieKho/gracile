#ifndef PART_HPP
#define PART_HPP

template <class = void>
class Part
{
public:
private:
public:
    virtual ~Part() = 0;

    virtual auto Start() -> void {}
    virtual auto Process() -> void {}
    virtual auto Draw() -> void {}
    virtual auto Finish() -> void {}
};

template <>
Part<>::~Part() {}

#endif // PART_HPP