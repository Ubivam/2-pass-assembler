#ifndef SELECTION_H
#define SELECTION_H

class Selection{
    public:
        Selection(std::string name, uint32_t index uint32_t locctr=0, bool org = false);

        void incLocationCounter(uint32_t amount);

        void resetLocationCounter();
}
#endif