#include "dice_pool.hpp"

bool dice::dice_pool::is_init = false;
std::mt19937 dice::dice_pool::gen;
std::uniform_int_distribution<> dice::dice_pool::dis(1, 6);

dice::dice_pool::dice_pool() :
    freq_tab{0, 0, 0, 0, 0, 0}
{
    if (!is_init)
    {
        is_init = true;
        std::random_device rd;
        gen = std::mt19937(rd());
        dis = std::uniform_int_distribution<>(0, 5);
    }
}

void dice::dice_pool::roll(const unsigned int count)
{
    if (count == 0) return;
    for (unsigned int i = 0; i < count; ++i)
    {
        unsigned int roll = dis(gen);
        ++freq_tab[roll];
    }
}

void dice::dice_pool::execute(const roll_order &order)
{
    roll(order.total_roll());
    if (order.homogeneity() == 0) return;

    unsigned int cut = order.cut_dice;
    unsigned int drop = order.drop_dice;
    for (auto i = freq_tab.rbegin(); i != freq_tab.rend(); ++i)
    {
        count_t &v = *i;
        if (v >= cut)
        {
            v -= cut;
            break;
        }
        else
        {
            cut -= v;
            v = 0;
        }
    }
    for (auto i = freq_tab.begin(); i != freq_tab.end(); ++i)
    {
        count_t &v = *i;
        if (v >= drop)
        {
            v -= drop;
            break;
        }
        else
        {
            drop -= v;
            v = 0;
        }
    }
}