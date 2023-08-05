//
// Created by igor on 14/06/2023.
//

#include "Model.h"

std::unique_ptr<Model> Model::model = nullptr;

void Model::go() {
    for (const auto &object : *Agent_list) {
        object->update();
    }
    time++;
}

void Model::addKnight(const std::string &name, const std::string &home) {
    auto it = findStructure(home);
    if (!it) {
        throw std::exception();
    }
    double x = it->getLocation().x;
    double y = it->getLocation().y;

    std::shared_ptr<Knight> knight =
        std::make_shared<Knight>(name, Point(x, y));

    Sim_object_list->push_back(knight);
    Agent_list->push_back(knight);
}

void Model::addPeasant(const std::string &name, Point position) {
    std::shared_ptr<Peasant> peasant =
        std::make_shared<Peasant>(name, position);
    Sim_object_list->push_back(peasant);
    Agent_list->push_back(peasant);
}

void Model::addThug(const std::string &name, Point position) {
    std::shared_ptr<Thug> thug = std::make_shared<Thug>(name, position);
    Sim_object_list->push_back(thug);
    Agent_list->push_back(thug);
}

std::shared_ptr<Structure> Model::findStructure(const std::string &name) {
    auto it = find_if(Structure_list->begin(), Structure_list->end(),
                      [&name](std::shared_ptr<Structure> &structure) {
                          return name == structure->getName();
                      });
    if (it != Structure_list->end()) {
        return *it;
    }
    return nullptr;
}

void Model::status() {
    for (const auto &obj : *Sim_object_list) {
        obj->broadcast_current_state();
        std::cout << std::endl;
    }
}

void Model::course(const std::string &basicString, double theta, int speed) {

    if (speed <= 0) {
        badInput("speed must be positive");
        return;
    } else if (speed > 30) {
        badInput("speed cant be bigger than 30");
        return;
    }

    auto agent = findAgent(basicString);
    std::shared_ptr<Thug> thug = std::dynamic_pointer_cast<Thug>(agent);
    if (thug) {
        thug->setCourse(theta);
        thug->setSpeed(speed);
    } else {
        m_view->Log("No thug named " + basicString);
    }
}

void Model::course(const std::string &basicString, double theta) {

    auto agent = findAgent(basicString);
    if (agent && !std::dynamic_pointer_cast<Peasant>(agent)) {
        agent->setCourse(theta);
    } else {
        m_view->Log("No agent named " + basicString);
    }
}

void Model::position(const std::string &basicString, Point point, int speed) {
    auto agent = findAgent(basicString);
    if (agent && std::dynamic_pointer_cast<Thug>(agent)) {
        agent->setDestinationCoordinates(point);
        agent->setSpeed(speed);
    } else {
        m_view->Log("No agent named " + basicString);
    }
}

void Model::position(const std::string &basicString, Point point) {
    auto agent = findAgent(basicString);
    if (agent && std::dynamic_pointer_cast<Knight>(agent)) {
        agent->setDestinationCoordinates(point);
    } else {
        m_view->Log("No agent named " + basicString);
    }
}

void Model::stop(const std::string &basicString) {
    auto agent = findAgent(basicString);
    if (agent) {
        agent->stop();
    } else {
        m_view->Log("No agent named " + basicString);
    }
}
void Model::destination(const std::string &basicString,
                        const std::string &castle) {
    auto agent = findAgent(basicString);
    auto castle_ = findStructure(castle);
    if (!castle_) {
        badInput("No structure named " + castle);
    }
    if (!agent) {
        badInput("No agent named " + basicString);
    }
    std::shared_ptr<Knight> knight = std::dynamic_pointer_cast<Knight>(agent);
    if (knight) {
        knight->setOnPatrol(castle_, Structure_list);
    } else {
        badInput(basicString + " not a knight");
    }
}

void Model::attack(const std::string &thug, const std::string &peasant) {
    // find if thug exists
    std::shared_ptr<Agent> thug_ = findAgent(thug);
    std::shared_ptr<Thug> thug1;
    if (thug_) {
        thug1 = std::dynamic_pointer_cast<Thug>(thug_);
        if (!thug1) {
            m_view->Log(thug + " is not a thug");
            return;
        }
    } else {
        m_view->Log("No agent named " + thug);
        return;
    }

    // find if peasant exists
    std::shared_ptr<Agent> peasant_ = findAgent(peasant);
    if (!peasant_) {
        m_view->Log("No agent named " + peasant);
        return;
    }
    std::shared_ptr<Peasant> peasant1 =
        std::dynamic_pointer_cast<Peasant>(peasant_);
    if (!peasant1) {
        m_view->Log(peasant + " is not a peasant");
        return;
    }
    thug1->attack(peasant1, Agent_list);
}

std::shared_ptr<Agent> Model::findAgent(const std::string &a_name) {
    auto it = std::find_if(Agent_list->begin(), Agent_list->end(),
                           [a_name](std::shared_ptr<Agent> &agent) {
                               return agent->getName() == a_name;
                           });
    // if it points to end() then it is nullptr
    return *it;
}

void Model::badInput(const std::string &str) {
    std::string badInputMessage = std::string("Bad input: \"") + str + "\"";
    m_view->Log(badInputMessage);
}

void Model::start_working(const std::string &peasant_name,
                          const std::string &farm_name,
                          const std::string &castle_name) {
    auto agent = findAgent(peasant_name);
    auto structure1 = findStructure(farm_name);
    auto structure2 = findStructure(castle_name);
    if (agent && structure1 && structure2) {
        std::shared_ptr<Peasant> peasant =
            std::dynamic_pointer_cast<Peasant>(agent);
        std::shared_ptr<Farm> farm =
            std::dynamic_pointer_cast<Farm>(structure1);
        std::shared_ptr<Castle> castle =
            std::dynamic_pointer_cast<Castle>(structure1);
        if (peasant && farm && castle) {
            peasant->start_working(farm, castle);
        }

        else {
            m_view->Log("Invalid name");
        }

    } else {
        m_view->Log("Invalid name");
    }
}
const std::shared_ptr<std::vector<std::shared_ptr<Sim_object>>> &
Model::getSimObjectList() const {
    return Sim_object_list;
}

void Model::makeDefault() { m_view->makeDefault(); }

void Model::setSizeView(int _size) { m_view->setSize(_size); }

void Model::setZoomView(double zoom) { m_view->setScale(zoom); }

void Model::setPanView(double x, double y) { m_view->setPan(x, y); }

void Model::show() { m_view->show(); }

void Model::attach(std::unique_ptr<View> someView) {
    if (!m_view) {
        m_view = std::move(someView);
    }
}

void Model::detach(std::unique_ptr<View> someView) {
    if (someView == m_view) {
        m_view.reset();
    }
}

void Model::addFarm(const std::string &line) {
    std::vector<std::string> &&separated = utils::split(line, ',');
    std::string &name = separated[0];
    std::string &xstr = separated[1];
    std::string &ystr = separated[2];
    std::string &hay = separated[3];
    std::string &prod = separated[4];

    xstr = xstr.substr(2);                    // to cut off " ("
    ystr = ystr.substr(0, ystr.length() - 1); // to cut off ")"

    double x = std::stod(xstr);
    double y = std::stod(ystr);

    int nhay = std::stoi(hay);

    int production = std::stoi(prod);

    std::shared_ptr<Farm> farm =
        std::make_shared<Farm>(name, Point(x, y), nhay, production);
    Structure_list->push_back(farm);
    Sim_object_list->push_back(farm);
}

void Model::addCastle(const std::string &line) {
    std::vector<std::string> &&separated = utils::split(line, ',');
    std::string &name = separated[0];
    std::string &xstr = separated[1];
    std::string &ystr = separated[2];
    std::string &hay = separated[3];

    xstr = xstr.substr(2);                    // to cut off " ("
    ystr = ystr.substr(0, ystr.length() - 1); // to cut off ")"

    double x = std::stod(xstr);
    double y = std::stod(ystr);

    int nhay = std::stoi(hay);

    std::shared_ptr<Castle> castle =
        std::make_shared<Castle>(name, Point(x, y), nhay);
    Structure_list->push_back(castle);
    Sim_object_list->push_back(castle);
}

void Model::log(const std::string& str) { m_view->Log(str); }

Model &Model::Get() {
    if (!model) {
        model = std::unique_ptr<Model>(new Model());
    }
    return *model;
}

Model::Model() : time(0) {
    Sim_object_list =
        std::make_shared<std::vector<std::shared_ptr<Sim_object>>>();
    Structure_list =
        std::make_shared<std::vector<std::shared_ptr<Structure>>>();
    Agent_list = std::make_shared<std::vector<std::shared_ptr<Agent>>>();
    m_view = std::make_unique<View>();
    m_view->addObjects(Sim_object_list);
}

const int &Model::getTime() { return time; };
