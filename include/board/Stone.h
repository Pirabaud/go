//
// Created by pirabaud on 6/19/25.
//

#ifndef STONE_H
#define STONE_H



class Stone {
public:
    [[nodiscard]] int getCol() const;
    [[nodiscard]] int getRow() const;
    Stone(int row, int col);
    ~Stone() = default;
private:
    int row;
    int col;

};



#endif //STONE_H
