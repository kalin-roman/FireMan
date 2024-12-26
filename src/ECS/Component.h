#ifndef COMPONENT_H
#define COMPONENT_H

struct IComponent
{
    protected:
        static int nextId;
};

template<typename T>
class Component: public IComponent {
    public:
        static int GetId() {
            static auto Id = nextId++;
            return Id;
    };
};


#endif
