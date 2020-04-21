#include <iostream>
#include <vector>
#include "./symgroup.hpp"

template<int mod>
class ModuloInt
{
    public:
        ModuloInt(int i):value(i%mod){}
        const int value;

        ModuloInt operator*(const ModuloInt& rhs) const
        {
            return ModuloInt(this->value+rhs.value);
        }
};

class ModuloIntCompare
{
    public:
        template<typename ModType>
        bool operator()(const ModType& lhs, const ModType& rhs) const
        {
            return lhs.value==rhs.value;
        }
};

int main()
{
    constexpr int Mod=600;
    SymGroup<ModuloInt<Mod>,ModuloIntCompare> mod_group({ModuloInt<Mod>(1)});

    auto elements=mod_group.operations();
    for(const auto& e : elements)
    {
        std::cout<<e.value<<std::endl;
    }

    auto subgroups=find_subgroups(mod_group);

    for(const auto& s : subgroups)
    {
        for(const auto& e : s.operations())
        {
            std::cout<<e.value<<" ";
        }
        std::cout<<"--------------------------\n";
    }
    
    return 0;
}
