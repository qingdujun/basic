#ifndef MOSS_DESIGNPATTRENS_OBSERVER_OBSERVER_HPP
#define MOSS_DESIGNPATTRENS_OBSERVER_OBSERVER_HPP

#include <utility> //std::forward
#include <map>

template <typename Func>
class Events {
public:
    Events() {
    }

    ~Events() {
        connections_.clear();
    }

    int Connect(Func&& f) {
        return Assgin(f);
    }

    int Connect(Func const& f) {
        return Assgin(f);
    }

    void Disconnect(int key) {
        connections_.erase(key);
    }

    template <typename... N>
    void Notify(N&&... args) {
        for (auto& iter : connections_) 
            iter.second(std::forward<N>(args)...);//exec
    }
private:
    Events(Events const&) = delete;
    Events& operator=(Events const&) = delete;
private:
    template <typename F>
    int Assgin(F&& f) {
        int k = observer_id_++;
        connections_.emplace(k, std::forward<F>(f));
        return k;
    }
private:
    int observer_id_;
    std::map<int, Func> connections_;
};

#endif