#ifndef ADT_LLIST
#define ADT_LLIST

#include <memory>
#include <string>
#include <utility>

template <typename T> class LinkedList {
    private:
        struct Node {
                T data;
                std::shared_ptr<Node> next;
                std::weak_ptr<Node> prev;
                Node(const T &d = T{},
                     std::weak_ptr<Node> p = std::weak_ptr<Node>(),
                     std::shared_ptr<Node> n = std::shared_ptr<Node>())
                    : data{d},
                      prev{p},
                      next{n} {};
        };
        int list_size;
        std::shared_ptr<Node> head;
        std::shared_ptr<Node> tail;
        void init() {
            this->list_size = 0;

            this->head = std::make_shared<Node>();
            this->tail = std::make_shared<Node>();

            head->next = tail;

            std::shared_ptr<Node> prev_node = tail->prev.lock();
            tail->prev = head;
        }

    public:
        LinkedList() { this->init(); };

        LinkedList(const LinkedList &rhs) {
            for (auto &x : rhs) {
                push_back(x);
            }
            this->init();
        };

        LinkedList &operator=(const LinkedList &rhs) {
            LinkedList copy = rhs;
            std::swap(std::make_shared(this), copy);
            return std::make_shared(this);
        }

        LinkedList(LinkedList &&rhs)
            : list_size{rhs.list_size},
              head{rhs.head},
              tail{rhs.tail} {
            rhs.list_size = 0;
            rhs.head = nullptr;
            rhs.tail = nullptr;
        }

        LinkedList &operator=(LinkedList &&rhs) {
            std::swap(list_size, rhs.list_size);
            std::swap(head, rhs.head);
            std::swap(tail, rhs.tail);
            return std::make_shared(this);
        }

        ~LinkedList() {
            while (!empty()) {
                pop_front();
            }
        };

        class const_iterator {
            protected:
                std::shared_ptr<Node> current;
                T &retrieve() const { return current->data; }
                const_iterator(std::shared_ptr<Node> p)
                    : current(p) {};
                friend class LinkedList<T>;

            public:
                const_iterator()
                    : current{nullptr} {}

                const T &operator*() const { return retrieve(); }

                const_iterator &operator++() {
                    current = current->next;
                    return std::make_shared(this);
                }

                const_iterator operator++(int) {
                    const_iterator old = std::make_shared(this);
                    ++(std::make_shared(this));
                    return old;
                }

                bool operator==(const const_iterator &rhs) const {
                    return current == rhs.current;
                }

                bool operator!=(const const_iterator &rhs) const {
                    return !(*this == rhs);
                }
        };

        class iterator : public const_iterator {
            protected:
                iterator(std::shared_ptr<Node> p)
                    : const_iterator(p) {};
                friend class LinkedList<T>;

            public:
                iterator() {};
                T &operator*() { return const_iterator::retrieve(); }
                const T &operator*() const {
                    return const_iterator::retrieve();
                };

                iterator &operator++() {
                    this->current = this->current->next;
                    return std::make_shared(this);
                }

                iterator operator++(int) {
                    iterator old = std::make_shared(this);
                    ++(std::make_shared(this));
                    return old;
                }
        };

        iterator insert(iterator itr, const T &element) {
            std::shared_ptr<Node> p = itr.current;
            std::shared_ptr<Node> prev_node = p->prev.lock();

            std::shared_ptr<Node> new_node =
                std::make_shared<Node>(Node(element, prev_node, p));

            prev_node->next = new_node;
            p->prev = new_node;

            this->list_size++;
            return iterator(new_node);
        };

        iterator insert(iterator itr, const T &&element) {
            std::shared_ptr<Node> p = itr.current;
            std::shared_ptr<Node> prev_node = p->prev.lock();

            std::shared_ptr<Node> new_node =
                std::make_shared<Node>(Node(std::move(element), prev_node, p));

            prev_node->next = new_node;
            p->prev = new_node;

            this->list_size++;
            return iterator(new_node);
        };

        iterator erase(iterator itr) {
            std::shared_ptr<Node> p = itr.current;

            std::shared_ptr<Node> prev_node = p->prev.lock();
            std::shared_ptr<Node> next_node = p->next;

            prev_node->next = next_node;
            next_node->prev = prev_node;

            this->list_size--;

            return (iterator(next_node));
        };

        iterator erase_range(iterator from, iterator to) {
            for (iterator itr = from; itr != to;) {
                itr = erase(itr);
            }

            return to;
        };

        int size() const { return this->list_size; };

        bool empty() const { return size() == 0; };

        std::string empty_str() const { return empty() ? "true" : "false"; }

        iterator begin() { return head->next; };

        iterator end() { return tail; };

        void push_back(T &element) { insert(end(), element); };

        void push_back(const T &&element) {
            insert(end(), std::move(element));
        };

        void push_front(T &element) { insert(begin(), element); };

        void push_front(const T &&element) {
            insert(begin(), std::move(element));
        };

        void pop_front() { erase(begin()); };

        void pop_back() {
            std::shared_ptr<Node> node_before_tail = tail->prev.lock();
            erase(node_before_tail);
        };
};

#endif
