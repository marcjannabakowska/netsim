
// Created by ZoJa on 14.12.2021.


#ifndef NET_SIM_FACTORY_HPP
#define NET_SIM_FACTORY_HPP

#include "storage_types.hpp"
#include "nodes.hpp"
#include <list>


enum NodeColor { UNVISITED, VISITED, VERIFIED };
bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors);


template <class Node>
class NodeCollection {
public:
    using container_t = typename std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    iterator begin() { return container.begin(); }
    iterator end() { return container.end(); }
    const_iterator cbegin() const { return container.cbegin(); }
    const_iterator cend() const { return container.cend(); }


private:
    container_t container;

public:

    void add(Node&& node) {container.push_back(std::move(node));}
    void remove_by_id(ElementID id)
    {
        container.remove_if([id](const Node& elem) {return elem.get_id() == id;}); //container erase(container.begin()+id);

    }
    NodeCollection<Node>::iterator find_by_id(ElementID id)
    {
        return std::find_if(container.begin(), container.end(), [id](const Node& elem) {return elem.get_id() == id;});
    }

    NodeCollection<Node>::const_iterator find_by_id(ElementID id) const
    {
        return std::find_if(container.begin(), container.end(), [id](const Node& elem) {return elem.get_id() == id;});
    };

};
class Factory {
private:
    template<class Node>
    void remove_receiver(NodeCollection<Node>& collection, ElementID id);
    NodeCollection<Ramp> cont_r;
    NodeCollection<Worker> cont_w;
    NodeCollection<Storehouse> cont_s;
public:

    void add_ramp(Ramp&& r) {cont_r.add(std::move(r));}
    void remove_ramp(ElementID id) {cont_r.remove_by_id(id);}
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) {return cont_r.find_by_id(id);}
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const {return cont_r.find_by_id(id);}
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const {return cont_r.cbegin();}
    NodeCollection<Ramp>::const_iterator  ramp_cend() const {return cont_r.cend();}


    void add_worker(Worker&& w) {cont_w.add(std::move(w));}
    void remove_worker(ElementID id)
    {
        Worker* node = &(*cont_w.find_by_id(id));
        std::for_each(cont_r.begin(), cont_r.end(), [&node](Ramp& ramp) {
            ramp.receiver_preferences_.remove_receiver(node);
        });

        std::for_each(cont_w.begin(), cont_w.end(), [&node](Worker& worker) {
            worker.receiver_preferences_.remove_receiver(node);
        });
        cont_w.remove_by_id(id);
    }
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) {return cont_w.find_by_id(id);}
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const {return cont_w.find_by_id(id);}
    NodeCollection<Worker>::const_iterator worker_cbegin() const {return cont_w.cbegin();}
    NodeCollection<Worker>::const_iterator  worker_cend() const {return cont_w.cend();}


    void add_storehouse(Storehouse&& s) {cont_s.add(std::move(s));}
    void remove_storehouse(ElementID id)
    {
        Storehouse* node = &(*cont_s.find_by_id(id));
        std::for_each(cont_w.begin(), cont_w.end(), [&node](Worker& ramp) {
            ramp.receiver_preferences_.remove_receiver(node);
        });

        std::for_each(cont_w.begin(), cont_w.end(), [&node](Worker& worker) {
            worker.receiver_preferences_.remove_receiver(node);
        });
        cont_s.remove_by_id(id);
    }
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) {return cont_s.find_by_id(id);}
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const {return cont_s.find_by_id(id);}
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const {return cont_s.cbegin();}
    NodeCollection<Storehouse>::const_iterator  storehouse_cend() const {return cont_s.cend();}

    bool is_consistent();
    void do_deliveries(Time);
    void do_package_passing();
    void do_work(Time);

};


template<class Node>
void Factory::remove_receiver(NodeCollection<Node>& collection, ElementID id) {

    auto iter = collection.find_by_id(id);
    //(*iter).get_preferences().
    auto receiver_ptr = dynamic_cast<IPackageReceiver*>(iter);

    for(auto & ramp : cont_r)
    {
        auto & _preferences = ramp.receiver_preferences_.get_preferences();
        for(auto _preference : _preferences)
        {
            if(_preference.first == receiver_ptr)
            {
                ramp.receiver_preferences_.remove_receiver(receiver_ptr);
                break;
            }
        }
    }

    for(auto & worker : cont_w)
    {
        auto & _preferences = worker.receiver_preferences_.get_preferences();
        for(auto _preference  : _preferences)
        {
            if(_preference.first == receiver_ptr)
            {
                worker.receiver_preferences_.remove_receiver(receiver_ptr);
                break;
            }
        }
    }
}

Factory load_factory_structure(std::istream& is);
void save_factory_structure( Factory& factory, std::ostream& os);
#endif //NET_SIM_FACTORY_HPP