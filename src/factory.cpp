//
// Created by ZoJa on 14.12.2021.
//

#include "factory.hpp"
#include <istream>
#include <string>
#include <sstream>


bool has_reachable_storehouse(/* const */PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors){
    if(node_colors[sender] == NodeColor::VERIFIED){
        return true;
    }
    node_colors[sender] = NodeColor::VISITED;
    if(sender->receiver_preferences_.get_preferences().empty()){
        throw std::logic_error("Sender has not got any receivers");
    }
    bool has_sender_at_least_one_receiver_other_than_himself = false;
    for(auto receiver : sender->receiver_preferences_.get_preferences()){
        if(receiver.first->get_receiver_type() == ReceiverType::STOREHOUSE){
            has_sender_at_least_one_receiver_other_than_himself = true;
        }
        else if(receiver.first->get_receiver_type() == ReceiverType::WORKER){
            IPackageReceiver* receiver_ptr = receiver.first;
            auto worker_ptr = dynamic_cast<class Worker*>(receiver_ptr);
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);
            if(sendrecv_ptr == sender){
                continue;
            }
            has_sender_at_least_one_receiver_other_than_himself = true;
            if(node_colors[sendrecv_ptr] == NodeColor::UNVISITED){
                has_reachable_storehouse(sendrecv_ptr,node_colors);
            }
        }
    }
    node_colors[sender] = NodeColor::VERIFIED;
    if(has_sender_at_least_one_receiver_other_than_himself){
        return true;
    }
    else{
        throw std::logic_error("Error");
    }
}

void Factory::do_deliveries(Time t) {
    for (auto &ramp : cont_r) {
        ramp.deliver_goods(t);
    }
}

void Factory::do_work(Time t ) {
    for (auto& worker : cont_w)
        worker.do_work(t);
}

void Factory::do_package_passing() {
    for (auto &ramp : cont_r) {
        ramp.send_package();
    }
    for (auto &worker : cont_w) {
        worker.send_package();
    }
}

bool Factory::is_consistent() {
    std::map<const PackageSender *, NodeColor> kolor;
    for (auto ptr = cont_w.begin(); ptr != cont_w.end(); ptr++){
        Worker* worker = &(*ptr);
        auto sender = dynamic_cast<PackageSender*>(worker);
        kolor[sender] = NodeColor::UNVISITED;
    }
    for (auto ptr = cont_r.begin(); ptr != cont_r.end(); ptr++){
        Ramp* ramp = &(*ptr);
        auto sender = dynamic_cast<PackageSender*>(ramp);
        kolor[sender] = NodeColor::UNVISITED;
    }
    try {
        for (auto ptr = cont_r.begin(); ptr != cont_r.end(); ptr++){
            Ramp* ramp = &(*ptr);
            auto sender = dynamic_cast<PackageSender*>(ramp);
            has_reachable_storehouse(sender, kolor);
        }
    }
    catch (const std::logic_error&) {
        return false;
    }
    return true;
}

enum class ElementType{
    RAMP,WORKER,STOREHOUSE,LINK
};
struct ParsedLineData{
    ElementType element_type;
    std::map<std::string, std::string> parameters;
};

std::vector<std::string> split(const std::string &text, char sep) {
    std::vector<std::string> tokens;
    std::size_t start = 0, end = 0;
    while ((end = text.find(sep, start)) != std::string::npos) {
        tokens.push_back(text.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(text.substr(start));
    return tokens;
}

ParsedLineData parse_line(std::string line) {
    std::vector<std::string> tokens;
    std::string token;

    std::istringstream token_stream(line);
    char delimiter = ' ';

    while (std::getline(token_stream, token, delimiter)) {
        tokens.push_back(token);
    }
    std::map<std::string, std::string> tag_map;
    std::vector<std::string> key_val;
    for(std::size_t i =1; i< std::size(tokens) ; i++){
        key_val = split(tokens[i],'=');
        tag_map.insert(std::pair<std::string,std::string>(key_val[0],key_val[1]));
    }
    ParsedLineData p;
    p.parameters = std::move(tag_map);
    if(tokens[0] == "LOADING_RAMP"){
        p.element_type = ElementType::RAMP;
    }else if(tokens[0] == "WORKER"){
        p.element_type = ElementType::WORKER;
    }else if(tokens[0] == "STOREHOUSE"){
        p.element_type = ElementType::STOREHOUSE;
    }else if(tokens[0] == "LINK"){
        p.element_type = ElementType::LINK;
    }

    return p;
}

Factory load_factory_structure(std::istream& is){
    Factory factory;
    std::string line;
    ParsedLineData p;
    std::list<Ramp>::iterator ramp;
    std::list<Worker>::iterator worker_in;
    std::list<Storehouse>::iterator store;
    std::list<Worker>::iterator worker_out;
    std::vector<std::string> key_val;
    while (std::getline(is, line)) {
        if(*line.data() != ';' and line != ""){
            p = parse_line(line);
            switch(p.element_type){
                case ElementType::RAMP:{
                    factory.add_ramp(Ramp(std::stoi(p.parameters["id"]),std::stoi(p.parameters["delivery-interval"])));
                    break;
                }
                case ElementType::WORKER:{
                    if(p.parameters["queue-type"] == "LIFO"){
                        factory.add_worker(Worker(std::stoi(p.parameters["id"]),std::stoi(p.parameters["processing-time"]),std::make_unique<PackageQueue>(PackageQueueType::LIFO)));
                    }else{
                        factory.add_worker(Worker(std::stoi(p.parameters["id"]),std::stoi(p.parameters["processing-time"]),std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
                    }
                    break;
                }
                case ElementType::STOREHOUSE:{
                    factory.add_storehouse(Storehouse(std::stoi(p.parameters["id"])));
                    break;
                }
                case ElementType::LINK:{

                    key_val = split(p.parameters["src"],'-');
                    if(key_val[0] == "ramp"){
                        ramp = factory.find_ramp_by_id(std::stoi(key_val[1]));

                        key_val = split(p.parameters["dest"],'-');
                        if(key_val[0] == "store"){
                            store = factory.find_storehouse_by_id(std::stoi(key_val[1]));
                            ramp->receiver_preferences_.add_receiver(&*store);
                        }else if(key_val[0] == "worker"){
                            worker_out = factory.find_worker_by_id(std::stoi(key_val[1]));
                            ramp->receiver_preferences_.add_receiver(&*worker_out);
                        }
                    }else if(key_val[0] == "worker"){
                        worker_in = factory.find_worker_by_id(std::stoi(key_val[1]));
                        key_val = split(p.parameters["dest"],'-');
                        if(key_val[0] == "store"){
                            store = factory.find_storehouse_by_id(std::stoi(key_val[1]));
                            worker_in->receiver_preferences_.add_receiver(&*store);
                        }else if(key_val[0] == "worker"){
                            worker_out = factory.find_worker_by_id(std::stoi(key_val[1]));
                            worker_in->receiver_preferences_.add_receiver(&*worker_out);
                        }
                    }


                    break;
                }
            }
        }
    }

    return factory;

}
void save_factory_structure( Factory& factory, std::ostream& os){
    os.flush();
    for(auto i = factory.ramp_cbegin(); i!= factory.ramp_cend(); i++){
        if(i == factory.ramp_cbegin()){
            os<<"LOADING_RAMP id="<<std::to_string(i->get_id())<<" delivery-interval="<<std::to_string(i->get_delivery_interval());

        }else{
            os<<"\n"<<"LOADING_RAMP id="<<std::to_string(i->get_id())<<" delivery-interval="<<std::to_string(i->get_delivery_interval());
        }

    }

    std::string queue_type;
    for(auto i = factory.worker_cbegin(); i!= factory.worker_cend(); i++){
        switch (i->get_queue()->get_queue_type()){
            case PackageQueueType::LIFO:{
                queue_type = "LIFO";
                break;
            }
            case PackageQueueType::FIFO:{
                queue_type = "FIFO";
                break;
            }
            default:
                break;
        }
        os<<"\n"<<"WORKER id="<<std::to_string(i->get_id())<<" processing-time="<<std::to_string(i->get_processing_duration())<<" queue-type="<<queue_type;
    }
    for(auto i = factory.storehouse_cbegin(); i!= factory.storehouse_cend(); i++){
        os<<"\n"<<"STOREHOUSE id="<<std::to_string(i->get_id());
    }
    std::string receiver_type;
    for(auto i = factory.ramp_cbegin(); i!= factory.ramp_cend(); i++){
        for(auto it = i->receiver_preferences_.cbegin(); it!= i->receiver_preferences_.cend(); it++){
            switch (it->first->get_receiver_type()){
                case ReceiverType::WORKER:{
                    receiver_type = "worker";
                    break;
                }
                case ReceiverType::STOREHOUSE:{
                    receiver_type = "store";
                    break;
                }
                default:
                    break;
            }
            os<<"\n"<<"LINK src=ramp-"<<std::to_string(i->get_id())<<" dest="<<receiver_type<<"-"<<std::to_string(it->first->get_id());
        }
    }
    for(auto i = factory.worker_cbegin(); i!= factory.worker_cend(); i++){
        for(auto it = i->receiver_preferences_.cbegin(); it!= i->receiver_preferences_.cend(); it++){
            switch (it->first->get_receiver_type()){
                case ReceiverType::WORKER:{
                    receiver_type = "worker";
                    break;
                }
                case ReceiverType::STOREHOUSE:{
                    receiver_type = "store";
                    break;
                }
                default:
                    break;
            }
            os<<"\n"<<"LINK src=worker-"<<std::to_string(i->get_id())<<" dest="<<receiver_type<<"-"<<std::to_string(it->first->get_id());
        }
    }
}

