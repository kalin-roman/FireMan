#ifndef POOL_H
#define POOL_H

class IPool{
    public:
        virtual ~IPool() = default;
        virtual void RemoveEntityFromPool(int entityId) = 0;
};
////////////////////////////////////////////////////////////////////////
//Pool
////////////////////////////////////////////////////////////////////////
//Just a vector (contiguous data) of object of type T
////////////////////////////////////////////////////////////////////////

template<typename T>
class Pool: public IPool
{
private:
    std::vector<T> data; 
    int size;

    //Helper maps to keep track of entities ids per index, so the vector is always packed
    std::unordered_map<int,int> entityIdToIndex;
    std::unordered_map<int,int> IndexToEntityId;


public:
    Pool(int capacity = 100) {
        size = 0;
        data.resize(capacity);
    };
    virtual ~Pool() = default;

    bool isEmpt() const{
        return size == 0;
    };

    int GetSize() const{
        return size;
    };

    void Resize(int n){
        data.resize(n);
    };

    void Add(T object){
        data.push_back(object);
    };

    void Set(int entityId, T object){
        if (entityIdToIndex.find(entityId) != entityIdToIndex.end()){
            int index = entityIdToIndex[entityId];
            data[index] = object;
        }else{
            int index = size;
            entityIdToIndex.emplace(entityId, index);
            IndexToEntityId.emplace(index, entityId);
            if (index >= data.capacity()){
                data.resize(size * 2);
            }
            data[index] = object;
            size++;
        }
    };

    void Remove(int entityId){
        int indexOfRemoved = entityIdToIndex[entityId];
        int indexOfLast = size - 1;
        data[indexOfRemoved] = data[indexOfLast];

        int entityIdOfLastElement = IndexToEntityId[indexOfLast];
        entityIdToIndex[entityIdOfLastElement] = indexOfRemoved;
        IndexToEntityId[indexOfRemoved] = entityIdOfLastElement;

        entityIdToIndex.erase(entityId);
        IndexToEntityId.erase(indexOfLast);

        size--;
    }

    void RemoveEntityFromPool(int entityId) override {
        if (entityIdToIndex.find(entityId) != entityIdToIndex.end()){
            Remove(entityId);
        }
    };


    void Clear(){
        data.clear();
        size = 0;
    };

    T& Get(int entityId){
        int index = entityIdToIndex[entityId];
        return static_cast<T&>(data[index]);
    };

    T& operator [](unsigned int index){
        return data[index];
    }


};
#endif
