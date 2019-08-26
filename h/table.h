#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include <memory>
template <class T>
class Table
{
private:
    std::vector<std::shared_ptr<T>> _table;

public:
    Table()
    {
    }
    Table(const Table &t)
    {
        _table = t->getTable();
    }
    Table &operator=(const Table &t)
    {
        _table = t->getTable();
    }
    std::vector<std::shared_ptr<T>> getTable()
    {
        return _table;
    }
    void insert(std::shared_ptr<T> symbol)
    {
        _table.push_back(symbol);
    }
    std::shared_ptr<T> find(std::string name)
    {
        for (auto &section : _table)
        {
            if (section.getName() == name)
            {
                return section;
            }
        }
        return 0;
    }
    std::shared_ptr<T> *begin()
    {
        return &_table[0];
    }
    const std::shared_ptr<T> *begin() const
    {
        return &_table[0];
    }
    std::shared_ptr<T> *end()
    {
        return &_table[size() - 1];
    }
    const std::shared_ptr<T> *end() const
    {
        return &_table[size() - 1];
    }
    std::size_t size() const
    {
        return _table.size();
    }
    friend std::ostream &operator<<(std::ostream *os, const Table<T> &table)
    {
        return os << table.to_string();
    }
    std::shared_ptr<T> &operator[](uint32_t index)
    {
        return _table[index];
    }
    std::string to_string() const
    {
        std::string ret = "";
        for (auto &entry : _table)
        {
            ret += (*entry).to_string() + "\n";
        }
        return ret;
    }
};

#endif