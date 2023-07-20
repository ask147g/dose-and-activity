#ifndef HIT_HH
#define HIT_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

class ActivityHit : public G4VHit {   
public:
    ActivityHit() {};
    ~ActivityHit() {};

    virtual void Draw() {};
    virtual void Print() {};

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);

    void SetName(G4String particle) {name = particle;}
    G4String GetName() {return name;}
    void SetLifeTime(G4double life) {lifeTime = life;}
    G4double GetLifeTime() {return lifeTime;}
    void SetProcess(G4String proc) {process = proc;}
    G4String GetProcess() {return process;}

    void SetNumber(G4int num) {number = num;}
    G4int GetNumber() {return number;}

private:
    G4String name;
    G4String process;
    G4double lifeTime;
    G4int number;
};

typedef G4THitsCollection<ActivityHit> HitsCollection;

extern G4ThreadLocal G4Allocator<ActivityHit>* ActivityHitAllocator;

inline void* ActivityHit::operator new(size_t)
{
  if(!ActivityHitAllocator) ActivityHitAllocator = new G4Allocator<ActivityHit>;
  return (void *) ActivityHitAllocator->MallocSingle();
}

inline void ActivityHit::operator delete(void *aHit)
{
  ActivityHitAllocator->FreeSingle((ActivityHit*) aHit);
}
#endif