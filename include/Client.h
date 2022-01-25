//
// Created by Wojtek on 07/01/2022.
//

#ifndef SPADE_ALGORITHM_CLIENT_H
#define SPADE_ALGORITHM_CLIENT_H


#include <cstddef>



class Client {
    int id;
public:
    Client(int id);

    bool operator==(const Client &rhs) const;

    bool operator!=(const Client &rhs) const;

    bool operator==(const int &rhs) const;

    bool operator!=(const int &rhs) const;

    bool operator<(const Client &rhs) const;

    bool operator>(const Client &rhs) const;

    bool operator<=(const Client &rhs) const;

    bool operator>=(const Client &rhs) const;

    int getId() const;

};

template<>
struct std::hash<Client>
{
    std::size_t operator()(Client const& c) const noexcept
    {
        return std::hash<int>{}(c.getId());
    }
};

#endif //SPADE_ALGORITHM_CLIENT_H
