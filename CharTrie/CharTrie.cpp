// CharTrie.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>

#include "TrieData.h"

int main()
{
    setlocale(LC_ALL, "Russian");

	using string_type = TrieStrings::StringOfCharsZeroEnd<char>;
    using trie_type = Trie::Trie<char, int>;
	
    string_type keys[] = { "альфа", "аЛьфа", "аМьфа", "аАьфа", "бета", "гамма", "дельта", "эпсилон" };

    trie_type tree1;
	std::cout << "-----------------------" << std::endl;
	std::cout << "добавление пар ключ/значение:" << std::endl;
    {
        int val = 1;
        for (const auto& key : keys)
        {
            std::cout << "tree1.addKeyValue(\"" << key.getStr() << "\", " << val << std::endl;
            tree1.addKeyValue(key, val++);

            std::cout << "\t\t-- состояние tree1 после добавления очередной записи:" << std::endl;
            for (auto it = tree1.cbegin(); it != tree1.cend(); ++it)
            {
                std::cout << "\t\t\t" << it.getString().getStr() << " : " << (*it)->getValue() << std::endl;
            }
        }
    }

    std::cout << "-----------------------" << std::endl;
	std::cout << "Переберем символы от 'а' до 'я':" << std::endl;
    for (char ch = 'а'; ch != 'я'; ++ch)
    {
        auto it = tree1.lower_bound(TrieStrings::StringOfCharsFixedLen<char>(&ch, 1));
        if (it != tree1.end())
        {
            std::cout << "Строки в tree1 старше символа '" << ch << "' :" << std::endl;
            for (; it != tree1.end(); ++it)
            {
                std::cout << "\t" << it.getString().getStr() << " : " << (*it)->getValue() << std::endl;
            }
        }
    }

    std::cout << "-----------------------" << std::endl;
    std::cout << "Получим значения элементов по ключам:" << std::endl;
    for (const auto& key : keys)
    {
        decltype(tree1)::const_iterator_type it = tree1.find(key);
        if (it != tree1.cend())
            std::cout << key.getStr() << " : " << (*it)->getValue() << std::endl;
        else
            std::cout << key.getStr() << " : ключ не найден!" << std::endl;
    }

    std::cout << "-----------------------" << std::endl;
    std::cout << "Обойдем пары ключ/значение в порядке возрастания ключей:" << std::endl;
    for (auto it = tree1.begin(); it != tree1.end(); ++it)
    {
        std::cout << it.getString().getStr() << " : " << (*it)->getValue() << std::endl;
    }

    std::cout << "-----------------------" << std::endl;
	std::cout << "Выполним удаление по ключам:" << std::endl;
    string_type keys1[] = { "аМьфа", "аМ", "ааМ", "бетЯ", "бет", "а" };
    for (const auto& key : keys1)
    {
        std::cout << "Удаление ключа \"" << key.getStr() << "\" - ";

        if (tree1.removeKey(key))
        {
            std::cout << "выполнено";
        }
        else
        {
            std::cout << "не выполнено";
        }
        std::cout << ";" << std::endl;
    }

    return 0;
}

