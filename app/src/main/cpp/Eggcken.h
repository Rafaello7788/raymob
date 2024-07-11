// Eggcken.h
#pragma once
#include "raymob.h"
#include "InputHandler.h"
#include <vector>

class Eggcken {
public:
    // Enums
    enum class State {
        Egg,
        Chicken,
        Gallus
    };

    // Structures
    struct GallusDomesticus {
        float x, y;
        float speedX, speedY;
        float radius;
        Color color;
        State state;
        float age;
        void update();
        void draw() const;
    };

    // Constants
    static const int MAX_GALLUSDOMESTICUS = 100;

    // Constructor and Destructor
    Eggcken(int numChickens = 4, int numGalluses = 1);
    ~Eggcken();

    // Public Methods
    void Update(const InputHandler& inputHandler);
    void Draw();

    // Getter Methods
    std::vector<Vector2> GetGallusDomesticusPositions() const;
    std::vector<float> GetGallusDomesticusRadii() const;
    Vector2 GetInteractGallusDomesticusPosition() const;
    float GetInteractGallusDomesticusRadius() const;

    // Debug Methods
    void PrintGallusDomesticusInfo() const;

private:
    // Private Methods
    void HandleGallusDomesticusCollision(GallusDomesticus& gallus1, GallusDomesticus& gallus2);
    void InitializeGallusDomesticus(int numChickens, int numGalluses);
    void UpdateGallusDomesticus(GallusDomesticus& gallus, float deltaTime);
    void CreateGallusDomesticus(State state);
    void CreateEgg(const Vector2& position);
    void RemoveOldestGallusDomesticus();

    // Utility Methods
    static int GetRandomValue(int min, int max);
    static Vector2 Vector2Normalize(Vector2 v);

    // Private Members
    std::vector<GallusDomesticus> gallusDomestici;
    GallusDomesticus* interactGallusDomesticus;
};