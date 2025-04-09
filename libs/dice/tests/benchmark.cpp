#include "dice"

#include <iostream>

int main() {
    unsigned int n = 100*1000;

    dice::dice_pool pool;
    dice::roll_order order
    {
        .clean_dice = 10,
        .cut_dice = 5,
        .drop_dice = 3
    };

    for (unsigned int i = 0; i < n; ++i) {
        pool.execute(order);
    }
}