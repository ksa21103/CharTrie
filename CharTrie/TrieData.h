#pragma once

#include <vector>
#include <iterator>
#include <cctype>

#include "TrieStrings.h"

namespace Trie
{
    // Получение неопределенного значения для типа данных, хранящегося в дереве
    template <typename TValueType>
    TValueType get_undefined_value()
    {
        static_assert("Не определено значение по умолчанию");
    }

    template <>
    int get_undefined_value()
    {
        return -1;
    }

    struct compare_no_case
    {
        bool operator()(char ch1, char ch2) const
        {
            return tolower(ch1) < tolower(ch2);
        }
    };

    ////////////////////////////////////////////////////////////////////////////
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    class Node
    {
    public:

        using node_type = Node<TCharType, TValueType, KeyCharLess>;

        static node_type* create();
        static node_type* create(TCharType keyChar);
        static node_type* create(TCharType keyChar, TValueType value);
        static node_type* clone (node_type* node);

        Node();
        Node(TCharType keyChar);
        Node(TCharType keyChar, TValueType value);
        virtual ~Node();

        ///////////////////////////////////////////
        // Работа с ключем

        // Установить/получить символ ключа
        void                setKeyChar(TCharType keyChar);
        TCharType           getKeyChar() const;

        ///////////////////////////////////////////
        // Работа со значением

        // Определить, хранится ли значение
        bool                haveValue() const;

        // Установить/получить значение
        void                setValue(TValueType value);
        const TValueType&   getValue() const;

        ///////////////////////////////////////////
        // Работа со следющими элементами

        // Установить/получить указатель на следующий элемент дерева
        void                setNext(node_type* next);
        node_type*          getNext() const;

        // Получить указатель на следующий элемент для указанного символа
        /**
         * Возращает элемент в цепочке элементов (братьев), обладающих указанным символом
         * Поиск начинается с текущего элемента
         */
        node_type*          getBrotherSimple(TCharType keyChar);
        const node_type*    getBrotherSimple(TCharType keyChar) const;

        // Получить указатель на следующий элемент для указанного символа
        /**
         * Возращает элемент в цепочке элементов (братьев), обладающих указанным символом
         * Поиск начинается с текущего элемента
         * Если подходящий элемент не найден - он будет создан
         * и размещен в цепочке элементов (братьев) в нужном месте
         */
        node_type*          getBrotherCreate(TCharType keyChar, bool& bCreated);

        // Изьять указатель на следующий элемент для указанного символа
        /**
         * Выполянется изъятие элемента для указанного символа из цепочки элементов нашего уровня
         * ВНИМАНИЕ: ответстенность за удаление элемента лежит на вызывающей стороне
         */
        node_type*          removeBrother(TCharType keyChar);

        ///////////////////////////////////////////
        // Работа с дочерним

        // Установить/получить указатель на дочерний элемент дерева
        void                setChild(node_type* child);
        node_type*          getChildSimple() const;
        node_type*          getChildCreate(bool& bCreated);

    private:

        bool                isLess(TCharType ch1, TCharType ch2) const;
        bool                isEqual(TCharType ch1, TCharType ch2) const;

    private:

        TCharType          m_keyChar = 0;
        TValueType         m_value   = get_undefined_value<TValueType>();
        node_type*         m_pNext   = nullptr;
        node_type*         m_pChild  = nullptr;
    };

    ////////////////////////////////////////////////////////////////////////////
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    class const_iterator;
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    class iterator;

    ////////////////////////////////////////////////////////////////////////////
    template<typename TCharType, typename TValueType, typename KeyCharLess = compare_no_case>
    class Trie
    {
    public:

        using string_type               = TrieStrings::StringOfChars<typename TCharType>;

        using node_type                 = Node<TCharType, TValueType, KeyCharLess>;
        using nodes_vector_type         = std::vector<node_type*>;

        using iterator_type             = iterator<typename TCharType, typename TValueType, typename KeyCharLess>;
        using const_iterator_type       = const_iterator<typename TCharType, typename TValueType, typename KeyCharLess>;

        Trie();
        ~Trie();

        // Добавление пары ключ/значение
        /**
         * Если ключ найден, то заменяем значение на новое.
         * Если ключ не найден, то добавляется новая пара ключ/значение
         */
        Node<typename TCharType, typename TValueType, typename KeyCharLess>* addKeyValue(const string_type& key, typename TValueType value);

        // Удаление заданного ключа (слова) из дерева
        /**
         * Удаляется и ключ, и значение
         */
        bool removeKey(const TrieStrings::StringOfChars<TCharType>& key);

        // Поиск заданного ключа (слова) в дереве
        /**
         * В случае, если слово найдено, возвращается значение.
         * Если слово не найдено, возвращается -1 (можно сделать любым другим способом)
         */
        iterator_type       find(const TrieStrings::StringOfChars<TCharType>& key);
        const_iterator_type find(const TrieStrings::StringOfChars<TCharType>& key) const;

        // Также, необходимо предоставить пример перебора всех значений для всех
        // слов больше заданного.
        // Пример:
        //      дерево содержит слова: {альфа, бета, гамма, дельта, эпсилон}.
        //      C каждым словом связано число.
        //      Нужно получить все числа для слов больше бета - т.е. найти числа,
        //      связанные со словами гамма, дельта, эпсилон. 
        const_iterator_type lower_bound(const TrieStrings::StringOfChars<TCharType>& key) const;

        const_iterator_type cbegin() const;
        const_iterator_type cend()   const;

    private:

        iterator_type       begin() const;
        iterator_type       end()   const;

        // Создание и получение корневого узла
        node_type*                          intGetRoot() const;

        // Удаление корневого узла
        void                                intResetRoot();

        // Получить узел для указанного значения ключа
        iterator_type                       intGetNodeSimple(const TrieStrings::StringOfChars<TCharType>& key, size_t keyLength);
        const_iterator_type                 intGetNodeSimple(const TrieStrings::StringOfChars<TCharType>& key, size_t keyLength) const;
        iterator_type                       intGetNodeCreate(const TrieStrings::StringOfChars<TCharType>& key, size_t keyLength);

        // Получить путь к узлу дереву по ключу
        nodes_vector_type                   intGetNodePathSimple(const TrieStrings::StringOfChars<TCharType>& key, size_t keyLength) const;

        // Получить путь к узлу дереву по ключу с созданием недостающих узлов
        nodes_vector_type                   intGetNodePathCreate(const TrieStrings::StringOfChars<TCharType>& key, size_t keyLength);

    private:

        // Корень цифрового дерева
        node_type* m_rootNode = Node<TCharType, TValueType, KeyCharLess>::create();
    };

    ////////////////////////////////////////////////////////////////////////////
    //
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    class base_interator : public std::iterator<std::forward_iterator_tag, Node<typename TCharType, typename TValueType, typename KeyCharLess>* >
    {
    public:

        using this_type         = base_interator<TCharType, TValueType, KeyCharLess>;

        using string_type       = TrieStrings::StringOfCharsZeroEnd<TCharType>;
        using node_type         = Node<typename TCharType, typename TValueType, typename KeyCharLess>;
        using nodes_vector_type = std::vector<node_type*>;

        base_interator();
        base_interator(const nodes_vector_type& nodes);
        base_interator(const this_type& other);
        base_interator(this_type&& other);

        bool                operator==  (const this_type& other) const;
        bool                operator!=  (const this_type& other) const;
        node_type*          operator->  ()                       const;
        node_type*          operator*   ()                       const;

        this_type&          operator=   (const this_type& other);
        this_type&          operator=   (this_type&& other);
        this_type&          operator++  ();

        // Получение полного ключ, которому соответствует узел
        const string_type&  getString() const;

	protected:

		struct TrieLevelInfo
		{
            TrieLevelInfo(node_type* _pNode, bool _bToChild)
                : pNode(_pNode), bToChild(_bToChild)
            { }

            node_type* pNode   = nullptr;  // Указатель на узел цифрового дерева
            bool       bToChild = false;    // Признак того, что при движении итератора вперед нужно заходить в дочерние элементы
		};
        using TrieLevelInfoVector = std::vector<TrieLevelInfo>;

        TrieLevelInfoVector m_path;         // Путь к узлу цифрового дерева, на который указывает итератор
        mutable string_type m_string;       // Ключ, которому соответствует узел
    };

    ////////////////////////////////////////////////////////////////////////////
    //
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    class const_iterator : public base_interator<TCharType, TValueType, KeyCharLess>
    {
        using base_iterator_type = base_interator<TCharType, TValueType, KeyCharLess>;
    public:

        using this_type         = const_iterator<TCharType, TValueType, KeyCharLess>;

        using base_type         = typename base_iterator_type::this_type;
        using node_type         = typename base_iterator_type::node_type;
        using nodes_vector_type = typename base_iterator_type::nodes_vector_type;

        const_iterator();
        const_iterator(const nodes_vector_type& nodes);
        const_iterator(const this_type& other);
        const_iterator(this_type&& other);

        bool                operator==  (const this_type& other) const;
        bool                operator!=  (const this_type& other) const;
        node_type*          operator->  ()                       const;
        node_type*          operator*   ()                       const;

        this_type&          operator=   (const this_type& other);
        this_type&          operator=   (this_type&& other);
        this_type&          operator++  ();
    };

    ////////////////////////////////////////////////////////////////////////////
    //
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    class iterator : public const_iterator<TCharType, TValueType, KeyCharLess>
    {
        using base_iterator_type = const_iterator<TCharType, TValueType, KeyCharLess>;
    public:

        using this_type         = iterator<TCharType, TValueType, KeyCharLess>;

        using base_type         = typename base_iterator_type::this_type;
        using node_type         = typename base_iterator_type::node_type;
        using nodes_vector_type = typename base_iterator_type::nodes_vector_type;

        iterator();
        iterator(const nodes_vector_type& nodes);
        iterator(const this_type& other);
        iterator(this_type&& other);

        bool                operator==  (const this_type& other) const;
        bool                operator!=  (const this_type& other) const;
        node_type*          operator->  ()                       const;
        node_type*          operator*   ()                       const;

        this_type&          operator=   (const this_type& other);
        this_type&          operator=   (this_type&& other);
        this_type&          operator++  ();
    }; 
}   // namespace Trie

// Имплементация методов узла цифрового дерева
namespace Trie
{
    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename Node<TCharType, TValueType, KeyCharLess>::node_type*
    Node<TCharType, TValueType, KeyCharLess>::create()
    {
        return new Node<TCharType, TValueType, KeyCharLess>();
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename Node<TCharType, TValueType, KeyCharLess>::node_type*
    Node<TCharType, TValueType, KeyCharLess>::create(TCharType keyChar)
    {
        return new Node<TCharType, TValueType, KeyCharLess>(keyChar);
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename Node<TCharType, TValueType, KeyCharLess>::node_type*
    Node<TCharType, TValueType, KeyCharLess>::create(TCharType keyChar, TValueType value)
    {
        return new Node<TCharType, TValueType, KeyCharLess>(keyChar, value);
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename Node<TCharType, TValueType, KeyCharLess>::node_type*
    Node<TCharType, TValueType, KeyCharLess>::clone(typename Node<TCharType, TValueType, KeyCharLess>::node_type* node)
    {
        node_type* newNode = create(node->getKeyChar(), node->getValue());
        newNode->setNext(node->getNext());
        newNode->setChild(node->getChildSimple());

        return newNode;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    Node<TCharType, TValueType, KeyCharLess>::Node()
    {
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    Node<TCharType, TValueType, KeyCharLess>::Node(TCharType keyChar)
        : m_keyChar (keyChar)
    {
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    Node<TCharType, TValueType, KeyCharLess>::Node(TCharType keyChar, TValueType value)
        : m_keyChar (keyChar),
          m_value   (value)
    {
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    Node<TCharType, TValueType, KeyCharLess>::~Node()
    {
        delete m_pChild;
        m_pChild = nullptr;

        delete m_pNext;
        m_pNext = nullptr;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    void
    Node<TCharType, TValueType, KeyCharLess>::setKeyChar(TCharType keyChar)
    {
        m_keyChar = keyChar;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    inline
    TCharType
    Node<TCharType, TValueType, KeyCharLess>::getKeyChar() const
    {
        return m_keyChar;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    bool
    Node<TCharType, TValueType, KeyCharLess>::haveValue() const
    {
        return m_value != get_undefined_value<TValueType>();
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    void
    Node<TCharType, TValueType, KeyCharLess>::setValue(TValueType value)
    {
        m_value = value;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    inline
    const TValueType&
    Node<TCharType, TValueType, KeyCharLess>::getValue() const
    {
        return m_value;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    void
    Node<TCharType, TValueType, KeyCharLess>::setNext(typename Node<TCharType, TValueType, KeyCharLess>::node_type* next)
    {
        m_pNext = next;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    inline
    typename Node<TCharType, TValueType, KeyCharLess>::node_type*
    Node<TCharType, TValueType, KeyCharLess>::getNext() const
    {
        return m_pNext;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename Node<TCharType, TValueType, KeyCharLess>::node_type*
    Node<TCharType, TValueType, KeyCharLess>::getBrotherSimple(TCharType keyChar)
    {
        node_type* destNode = nullptr;

        if (isLess(keyChar, getKeyChar()))
        {
            // Символ меньше символа в текущем элементе - нужного элемента нет
        }

        // Если символ из ключа хранится в текущем элементе - вернем текущий элемент
        else if (isEqual(keyChar, getKeyChar()))
        {
            return this;
        }
        else
        {
            node_type* nodePrev = this;
            node_type* node = getNext();
            while (node && isLess(node->getKeyChar(), keyChar))
            {
                nodePrev = node;
                node = node->getNext();
            }

            if (node)
            {
                if (isEqual(node->getKeyChar(), keyChar))
                {
                    destNode = node;
                }
            }
        }

        return destNode;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    const typename Node<TCharType, TValueType, KeyCharLess>::node_type*
    Node<TCharType, TValueType, KeyCharLess>::getBrotherSimple(TCharType keyChar) const
    {
        const node_type* destNode = nullptr;

        if (isLess(keyChar, getKeyChar()))
        {
            // Символ меньше символа в текущем элементе - нужного элемента нет
        }

        // Если символ из ключа хранится в текущем элементе - вернем текущий элемент
        else if (isEqual(keyChar, getKeyChar()))
        {
            return this;
        }
        else
        {
            const node_type* nodePrev = this;
            const node_type* node = getNext();
            while (node && isLess(node->getKeyChar(), keyChar))
            {
                nodePrev = node;
                node = node->getNext();
            }

            if (node)
            {
                if (isEqual(node->getKeyChar(), keyChar))
                {
                    destNode = node;
                }
            }
        }

        return destNode;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename Node<TCharType, TValueType, KeyCharLess>::node_type*
    Node<TCharType, TValueType, KeyCharLess>::getBrotherCreate(TCharType keyChar, bool& bCreated)
    {
        node_type* destNode = nullptr;
        bCreated = false;

        if (isLess(keyChar, getKeyChar()))
        {
            node_type* newNode = Node::clone(this);
            setNext(newNode);
            setChild(nullptr);

            setKeyChar(keyChar);
            setValue(get_undefined_value<TValueType>());

            destNode = this;

            bCreated = true;
        }

        // Если символ из ключа хранится в текущем элементе - вернем текущий элемент
        else if (isEqual(keyChar, getKeyChar()))
        {
            return this;
        }
        else
        {
            node_type* nodePrev = this;
            node_type* node = getNext();
            while (node && isLess(node->getKeyChar(), keyChar))
            {
                nodePrev = node;
                node = node->getNext();
            }

            if (node)
            {
                if (isEqual(node->getKeyChar(), keyChar))
                {
                    destNode = node;
                }
                else
                {
                    node_type* newNode = Node::create(keyChar);
                    node_type* oldNext = nodePrev->getNext();
                    nodePrev->setNext(newNode);
                    newNode->setNext(oldNext);

                    destNode = newNode;

                    bCreated = true;
                }
            }
            else
            {
                node_type* newNode = Node::create(keyChar);
                nodePrev->setNext(newNode);

                destNode = newNode;

                bCreated = true;
            }
        }

        return destNode;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename Node<TCharType, TValueType, KeyCharLess>::node_type*
        Node<TCharType, TValueType, KeyCharLess>::removeBrother(TCharType keyChar)
    {
        node_type* result = nullptr;

        if (m_pChild && isEqual(m_pChild->getKeyChar(), keyChar))
        {
            result = m_pChild;
            m_pChild = m_pNext;
            m_pNext = m_pNext ? m_pNext->getNext() : nullptr;
        }
        else
        {
            node_type* nodePrev = nullptr;
            for (node_type* node = m_pNext; node != nullptr; node = node->getNext())
            {
                if (isEqual(node->getKeyChar(), keyChar))
                {
                    result = node;

                    if (nodePrev)
                        nodePrev->setNext(result->getNext());
                    else
                        m_pNext = result->getNext();

                    result->setNext(nullptr);

                    break;
                }

                nodePrev = node;
            }
        }

        return result;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    void
    Node<TCharType, TValueType, KeyCharLess>::setChild(typename Node<TCharType, TValueType, KeyCharLess>::node_type* child)
    {
        m_pChild = child;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename Node<TCharType, TValueType, KeyCharLess>::node_type*
    Node<TCharType, TValueType, KeyCharLess>::getChildSimple() const
    {
        return m_pChild;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    inline
    typename Node<TCharType, TValueType, KeyCharLess>::node_type*
    Node<TCharType, TValueType, KeyCharLess>::getChildCreate(bool& bCreated)
    {
        bCreated = false;

        if (!m_pChild)
        {
            m_pChild = Node::create();
            bCreated = true;
        }

        return m_pChild;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    inline
    bool
    Node<TCharType, TValueType, KeyCharLess>::isLess(TCharType ch1, TCharType ch2) const
    {
        return ch1 == ch2 ? false : KeyCharLess()(ch1, ch2);
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    inline
    bool
    Node<TCharType, TValueType, KeyCharLess>::isEqual(TCharType ch1, TCharType ch2) const
    {
        KeyCharLess less;
        return ch1 == ch2 ? true : (!less(ch1, ch2) && !less(ch2, ch1));
    }

}   // namespace Trie (Имплементация методов узла цифрового дерева)

// Имплементация методов цифрового дерева
namespace Trie
{
    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    Trie<TCharType, TValueType, KeyCharLess>::Trie()
    {
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    Trie<TCharType, TValueType, KeyCharLess>::~Trie()
    {
        intResetRoot();
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    Node<TCharType, TValueType, KeyCharLess>*
    Trie<TCharType, TValueType, KeyCharLess>::addKeyValue(
        const string_type& key, TValueType value)
    {
        // Получим (и создадим недостающие элементы при необходимости) узел для указанного пути
        iterator_type it = intGetNodeCreate(key, key.length());
        assert(it != end());

        Node<TCharType, TValueType, KeyCharLess>* result = *it;

        // Установим значение
        result->setValue(value);

        return result;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    bool
    Trie<TCharType, TValueType, KeyCharLess>::removeKey(const TrieStrings::StringOfChars<TCharType>& key)
    {
        bool bResult = false;

        if (0 == key.length())
        {
            intResetRoot();
            bResult = true;
        }

        // Получим узел для указанного пути без последнего символа
        else
        {
            const size_t keyLenght = key.length();
            iterator_type it = intGetNodeSimple(key, keyLenght);
            if (it != end())
            {
                if (node_type* result = (*it)->removeBrother(key.at(keyLenght - 1)))
                {
                    delete result;
                    bResult = true;
                }
            }
        }

        return bResult;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename Trie<TCharType, TValueType, KeyCharLess>::iterator_type
    Trie<TCharType, TValueType, KeyCharLess>::find(const TrieStrings::StringOfChars<TCharType>& key)
    {
        return intGetNodeSimple(key, key.length());
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename Trie<TCharType, TValueType, KeyCharLess>::const_iterator_type
    Trie<TCharType, TValueType, KeyCharLess>::find(const TrieStrings::StringOfChars<TCharType>& key) const
    {
        return intGetNodeSimple(key, key.length());
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename Trie<TCharType, TValueType, KeyCharLess>::const_iterator_type
    Trie<TCharType, TValueType, KeyCharLess>::lower_bound(const TrieStrings::StringOfChars<TCharType>& key) const
    {
        auto it = intGetNodeSimple(key, key.length());
        if (it != end())
        {
            ++it;
        }

        return it;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename Trie<TCharType, TValueType, KeyCharLess>::const_iterator_type
    Trie<TCharType, TValueType, KeyCharLess>::cbegin() const
    {
        const_iterator_type it;

        if (node_type* firstChild = intGetRoot()->getChildSimple())
        {
            it = const_iterator_type({ firstChild });
            ++it;
        }

        return it;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename Trie<TCharType, TValueType, KeyCharLess>::const_iterator_type
    Trie<TCharType, TValueType, KeyCharLess>::cend() const
    {
        return const_iterator_type();
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename Trie<TCharType, TValueType, KeyCharLess>::iterator_type
    Trie<TCharType, TValueType, KeyCharLess>::begin() const
    {
        iterator_type it;

        if (node_type* firstChild = intGetRoot()->getChildSimple())
        {
            it = iterator_type({ firstChild, true });
            ++it;
        }

        return it;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename Trie<TCharType, TValueType, KeyCharLess>::iterator_type
    Trie<TCharType, TValueType, KeyCharLess>::end() const
    {
        return iterator_type();
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename Trie<TCharType, TValueType, KeyCharLess>::node_type*
    Trie<TCharType, TValueType, KeyCharLess>::intGetRoot() const
    {
        return m_rootNode;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    void
    Trie<TCharType, TValueType, KeyCharLess>::intResetRoot()
    {
        delete m_rootNode;
        m_rootNode = nullptr;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename Trie<TCharType, TValueType, KeyCharLess>::iterator_type
    Trie<TCharType, TValueType, KeyCharLess>::intGetNodeCreate(
        const TrieStrings::StringOfChars<TCharType>& key, size_t keyLength)
    {
        return iterator_type(intGetNodePathCreate(key, keyLength));
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename Trie<TCharType, TValueType, KeyCharLess>::iterator_type
    Trie<TCharType, TValueType, KeyCharLess>::intGetNodeSimple(
        const TrieStrings::StringOfChars<TCharType>& key, size_t keyLength)
    {
        return iterator_type(intGetNodePathSimple(key, keyLength));
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename Trie<TCharType, TValueType, KeyCharLess>::const_iterator_type
    Trie<TCharType, TValueType, KeyCharLess>::intGetNodeSimple(
        const TrieStrings::StringOfChars<TCharType>& key, size_t keyLength) const
    {
        return const_iterator_type(intGetNodePathSimple(key, keyLength));
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename Trie<TCharType, TValueType, KeyCharLess>::nodes_vector_type
    Trie<TCharType, TValueType, KeyCharLess>::intGetNodePathSimple(
        const TrieStrings::StringOfChars<TCharType>& key, size_t keyLength) const
    {
        nodes_vector_type path;
        path.reserve(keyLength);

        node_type* currentNode = intGetRoot();
        for (size_t keyCharIndex = 0; keyCharIndex < keyLength; ++keyCharIndex)
        {
            const TCharType keyChar = key.at(keyCharIndex);

            // Переходим на вложенный уровень
            currentNode = currentNode->getChildSimple();
            if (!currentNode)
                break;

            // Получим на вложенном уровне элемент для текущего символа ключа
            currentNode = currentNode->getBrotherSimple(keyChar);
            if (!currentNode)
                break;

            path.push_back(currentNode);
        }

        if (!currentNode)
            path.clear();

        return path;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename Trie<TCharType, TValueType, KeyCharLess>::nodes_vector_type
    Trie<TCharType, TValueType, KeyCharLess>::intGetNodePathCreate(
        const TrieStrings::StringOfChars<TCharType>& key, size_t keyLength)
    {
        nodes_vector_type path;
        path.reserve(keyLength);

        Node<TCharType, TValueType, KeyCharLess>* currentNode = intGetRoot();
        for (size_t keyCharIndex = 0; keyCharIndex < keyLength; ++keyCharIndex)
        {
            const TCharType keyChar = key.at(keyCharIndex);

            bool bCreated(false);

            // Переходим на вложенный уровень
            currentNode = currentNode->getChildCreate(bCreated);
            if (!currentNode)
                break;

            // Проставим текущий символ ключа в только что созданный элемент
            if (!bCreated)
            {
                // Создаем на вложенном уровне элемент для текущего символа ключа
                currentNode = currentNode->getBrotherCreate(keyChar, bCreated);
                if (!currentNode)
                    break;
            }

            // Проставим текущий символ ключа в только что созданный элемент
            if (bCreated)
            {
                currentNode->setKeyChar(keyChar);
            }

            path.push_back(currentNode);
        }

        if (!currentNode)
            path.clear();

        return path;
    }

}   // namespace Trie (Имплементация методов цифрового дерева)

// Имплементация итераторов цифрового дерева
namespace Trie
{
    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    base_interator<TCharType, TValueType, KeyCharLess>::base_interator()
    {
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    base_interator<TCharType, TValueType, KeyCharLess>::base_interator(
        const nodes_vector_type& nodes)
    {
        if (!nodes.empty())
        {
            m_path.reserve(nodes.size());
            for (node_type* node : nodes)
            {
                // Создадим новый элемент пути без признака захода в дочерние элементы
                m_path.push_back(TrieLevelInfo(node, false));
            }

            // Только в последнем элементе пути укажем, что при движении итератора вперед
            // нужно заходить в дочерние элементы
            m_path.back().bToChild = true;
        }
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    base_interator<TCharType, TValueType, KeyCharLess>::base_interator(const this_type& other)
        : m_path    (other.m_path),
          m_string  (other.m_string)
    {
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    base_interator<TCharType, TValueType, KeyCharLess>::base_interator(this_type&& other)
        : m_path    (std::move(other.m_path)),
          m_string  (std::move(other.m_string))
    {
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename base_interator<TCharType, TValueType, KeyCharLess>::node_type*
    base_interator<TCharType, TValueType, KeyCharLess>::operator->() const
    {
        return m_path.back().pNode;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename base_interator<TCharType, TValueType, KeyCharLess>::node_type*
    base_interator<TCharType, TValueType, KeyCharLess>::operator*() const
    {
        return m_path.back().pNode;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    const typename base_interator<TCharType, TValueType, KeyCharLess>::string_type&
    base_interator<TCharType, TValueType, KeyCharLess>::getString() const
    {
        if (m_string.empty() && !m_path.empty())
        {
            m_string.reserve(m_path.size() + 1);

            for (const TrieLevelInfo& item : m_path)
            {
                m_string.appendChar(item.pNode->getKeyChar());
            }
        }

        return m_string;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    base_interator<TCharType, TValueType, KeyCharLess>&
    base_interator<TCharType, TValueType, KeyCharLess>::operator=(const this_type& other)
    {
        m_path   = other.m_path;
        m_string = other.m_string;

        return *this;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    base_interator<TCharType, TValueType, KeyCharLess>&
    base_interator<TCharType, TValueType, KeyCharLess>::operator=(this_type&& other)
    {
        m_path   = std::move(other.m_path);
        m_string = std::move(other.m_string);

        return *this;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename base_interator<TCharType, TValueType, KeyCharLess>::this_type&
    base_interator<TCharType, TValueType, KeyCharLess>::operator++()
    {
        if (m_path.empty())
            return *this;

        bool bClearStr(false);

        do
        {
            TrieLevelInfo& levelInfo = m_path.back();

            while (true)
            {
                if (levelInfo.bToChild)
                {
                    levelInfo.bToChild = false;

                    if (node_type* node = levelInfo.pNode->getChildSimple())
                    {
                        // Создадим новый элемент пути с признаком захода в дочерние элементы
                        m_path.push_back(TrieLevelInfo(node, true));
                        bClearStr = true;
                        break;
                    }
                }

                if (node_type* node = levelInfo.pNode->getNext())
                {
                    m_path.pop_back();
                    // Создадим новый элемент пути с признаком захода в дочерние элементы
                    m_path.push_back(TrieLevelInfo(node, true));
                    bClearStr = true;
                    break;
                }

                // Переходим на уровень выше
                m_path.pop_back();
                bClearStr = true;

                break;
            }

        } while (!m_path.empty() && !m_path.back().pNode->haveValue());

        if (bClearStr)
        {
            m_string.clear();
        }

        return *this;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    bool
    base_interator<TCharType, TValueType, KeyCharLess>::operator==(
        const this_type& other) const
    {
        if (m_path.empty() && other.m_path.empty())
            return true;

        if (m_path.size() != other.m_path.size())
            return false;

        TrieLevelInfoVector::const_iterator itThis     = m_path.cbegin();
        TrieLevelInfoVector::const_iterator itEndThis  = m_path.cend();
        TrieLevelInfoVector::const_iterator itOther    = other.m_path.cbegin();
        for (; itThis != itEndThis; ++itThis, ++itOther)
        {
            if (itThis->pNode != itOther->pNode)
                return false;
        }

        return true;
    }
    
    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    bool
    base_interator<TCharType, TValueType, KeyCharLess>::operator!=(
        const this_type& other) const
    {
        return !operator==(other);
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    const_iterator<TCharType, TValueType, KeyCharLess>::const_iterator()
    {
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    const_iterator<TCharType, TValueType, KeyCharLess>::const_iterator(
        const nodes_vector_type& nodes)
        : base_type(nodes)
    {
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    const_iterator<TCharType, TValueType, KeyCharLess>::const_iterator(
        const this_type& other)
        : base_type(other)
    {
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    const_iterator<TCharType, TValueType, KeyCharLess>::const_iterator(
        this_type&& other)
        : base_type(other)
    {
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    bool
    const_iterator<TCharType, TValueType, KeyCharLess>::operator==(const this_type& other) const
    {
        return base_type::operator==(other);
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    bool
    const_iterator<TCharType, TValueType, KeyCharLess>::operator!=(const this_type& other) const
    {
        return base_type::operator!=(other);
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename const_iterator<TCharType, TValueType, KeyCharLess>::node_type*
    const_iterator<TCharType, TValueType, KeyCharLess>::operator->() const
    {
        return base_type::operator->();
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename const_iterator<TCharType, TValueType, KeyCharLess>::node_type*
    const_iterator<TCharType, TValueType, KeyCharLess>::operator*() const
    {
        return base_type::operator*();
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    const_iterator<TCharType, TValueType, KeyCharLess>&
    const_iterator<TCharType, TValueType, KeyCharLess>::operator=(const this_type& other)
    {
        base_type::operator=(other);

        return *this;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    const_iterator<TCharType, TValueType, KeyCharLess>&
    const_iterator<TCharType, TValueType, KeyCharLess>::operator=(this_type&& other)
    {
        base_type::operator=(std::move(other));

        return *this;
    }

    //-----------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    const_iterator<TCharType, TValueType, KeyCharLess>&
    const_iterator<TCharType, TValueType, KeyCharLess>::operator++()
    {
        base_type::operator++();

        return *this;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    iterator<TCharType, TValueType, KeyCharLess>::iterator()
    {
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    iterator<TCharType, TValueType, KeyCharLess>::iterator(
        const nodes_vector_type& nodes)
        : base_type(nodes)
    {
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    iterator<TCharType, TValueType, KeyCharLess>::iterator(const this_type& other)
        : base_type(other)
    {
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    iterator<TCharType, TValueType, KeyCharLess>::iterator(this_type&& other)
        : base_type(std::move(other))
    {
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename iterator<TCharType, TValueType, KeyCharLess>::node_type*
    iterator<TCharType, TValueType, KeyCharLess>::operator->() const
    {
        return base_type::operator->();
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    typename iterator<TCharType, TValueType, KeyCharLess>::node_type*
    iterator<TCharType, TValueType, KeyCharLess>::operator*() const
    {
        return base_type::operator*();
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    iterator<TCharType, TValueType, KeyCharLess>&
    iterator<TCharType, TValueType, KeyCharLess>::operator=(const iterator& other)
    {
        base_type::operator=(other);

        return *this;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    iterator<TCharType, TValueType, KeyCharLess>&
    iterator<TCharType, TValueType, KeyCharLess>::operator=(iterator&& other)
    {
        base_type::operator=(std::move(other));

        return *this;
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    bool
    iterator<TCharType, TValueType, KeyCharLess>::operator==(
        const iterator& other) const
    {
        return base_type::operator==(other);
    }

    //------------------------------------------------------------------------//
    template<typename TCharType, typename TValueType, typename KeyCharLess>
    bool
    iterator<TCharType, TValueType, KeyCharLess>::operator!=(const iterator& other) const
    {
        return !base_type::operator==(other);
    }

    //------------------------------------------------------------------------//

}   // namespace Trie (Имплементация итераторов цифрового дерева)
