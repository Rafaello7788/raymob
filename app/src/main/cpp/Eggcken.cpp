// Eggcken.cpp

// Mini Cheat Sheet:
// GetFrameTime(): Get time in seconds for last frame
// CheckCollisionCircles(): Check collision between two circles
// IsGestureDetected(): Check if a gesture has been detected
// GetTouchPosition(): Get touch position XY for a touch point
// IsCursorOnScreen(): Check if cursor is on the current screen

#include "Eggcken.h"
#include "raymob.h"
#include "raymath.h"
#include <cmath>
#include <random>
#include <algorithm>
#include <iostream>

// Section 1: Constructor and Destructor
// 1.1 Constructor
Eggcken::Eggcken(int numChickens, int numGalluses) : interactGallusDomesticus(nullptr) {
    InitializeGallusDomesticus(numChickens, numGalluses);
}

// 1.2 Destructor
Eggcken::~Eggcken() {
    if (interactGallusDomesticus) {
        delete interactGallusDomesticus;
        interactGallusDomesticus = nullptr;
    }
}

// 1.3 Initialize GallusDomesticus
void Eggcken::InitializeGallusDomesticus(int numChickens, int numGalluses) {
    for (int i = 0; i < numChickens; ++i) {
        CreateGallusDomesticus(State::Chicken);
    }
    for (int i = 0; i < numGalluses; ++i) {
        CreateGallusDomesticus(State::Gallus);
    }
}

// Section 2: GallusDomesticus Methods
// 2.1 Update GallusDomesticus
void Eggcken::GallusDomesticus::update() {
    x += speedX;
    y += speedY;
}

// 2.2 Draw GallusDomesticus
void Eggcken::GallusDomesticus::draw() const {
    DrawCircle(static_cast<int>(x), static_cast<int>(y), radius, color);
}

// 2.3 Create GallusDomesticus
void Eggcken::CreateGallusDomesticus(State state) {
    if (gallusDomestici.size() >= MAX_GALLUSDOMESTICUS) {
        std::cout << "Max GallusDomesticus reached. Removing oldest..." << std::endl;
        RemoveOldestGallusDomesticus();
    }

    const float speed = 2.0f;
    const float radius = 10.0f;
    
    float x = GetRandomValue(radius, GetScreenWidth() - radius);
    float y = GetRandomValue(radius, GetScreenHeight() - radius);
    float angle = GetRandomValue(0, 360) * DEG2RAD;
    
    GallusDomesticus newInstance = {
        x, y,
        speed * cosf(angle), speed * sinf(angle),
        radius,
        (state == State::Chicken) ? WHITE : RED,
        state,
        0.0f  // Initial age
    };
    gallusDomestici.push_back(newInstance);
    
    std::cout << "Created new GallusDomesticus. Total count: " << gallusDomestici.size() << std::endl;
}

// Section 3: Collision Handling
// 3.1 Handle GallusDomesticus Collision
void Eggcken::HandleGallusDomesticusCollision(GallusDomesticus& gallus1, GallusDomesticus& gallus2) {
    float dx = gallus2.x - gallus1.x;
    float dy = gallus2.y - gallus1.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance < gallus1.radius + gallus2.radius) {
        float nx = dx / distance;
        float ny = dy / distance;

        float relativeVelocityX = gallus2.speedX - gallus1.speedX;
        float relativeVelocityY = gallus2.speedY - gallus1.speedY;

        float dotProduct = relativeVelocityX * nx + relativeVelocityY * ny;

        if (dotProduct > 0) return; // GallusDomestici are moving away from each other

        float restitution = 0.9f; // coefficient of restitution
        float impulse = -(1 + restitution) * dotProduct / (1/gallus1.radius + 1/gallus2.radius);

        gallus1.speedX -= impulse * nx / gallus1.radius;
        gallus1.speedY -= impulse * ny / gallus1.radius;
        gallus2.speedX += impulse * nx / gallus2.radius;
        gallus2.speedY += impulse * ny / gallus2.radius;

        // Separate the GallusDomestici
        float overlap = (gallus1.radius + gallus2.radius - distance) / 2.0f;
        gallus1.x -= overlap * nx;
        gallus1.y -= overlap * ny;
        gallus2.x += overlap * nx;
        gallus2.y += overlap * ny;

        // Check for egg creation
        if ((gallus1.state == State::Chicken && gallus2.state == State::Gallus) ||
            (gallus1.state == State::Gallus && gallus2.state == State::Chicken)) {
            CreateEgg({(gallus1.x + gallus2.x) / 2, (gallus1.y + gallus2.y) / 2});
        }
    }
}

// Section 4: Update Methods
// 4.1 Main Update Method
void Eggcken::Update(const InputHandler& inputHandler) {
    float deltaTime = GetFrameTime();

    for (auto& gallus : gallusDomestici) {
        UpdateGallusDomesticus(gallus, deltaTime);
    }

    // Handle collisions between all GallusDomesticus instances
    for (size_t i = 0; i < gallusDomestici.size(); ++i) {
        for (size_t j = i + 1; j < gallusDomestici.size(); ++j) {
            HandleGallusDomesticusCollision(gallusDomestici[i], gallusDomestici[j]);
        }
    }

    // Maintain ratio (for example, 4:1 chickens to galluses)
    int chickenCount = std::count_if(gallusDomestici.begin(), gallusDomestici.end(),
        [](const GallusDomesticus& g) { return g.state == State::Chicken; });
    int gallusCount = std::count_if(gallusDomestici.begin(), gallusDomestici.end(),
        [](const GallusDomesticus& g) { return g.state == State::Gallus; });

    if (chickenCount < gallusCount * 4 && gallusDomestici.size() < MAX_GALLUSDOMESTICUS) {
        CreateGallusDomesticus(State::Chicken);
    } else if (gallusCount == 0 && gallusDomestici.size() < MAX_GALLUSDOMESTICUS) {
        CreateGallusDomesticus(State::Gallus);
    }

    // Handle touch interaction
    if (inputHandler.IsTouchPressed()) {
        if (interactGallusDomesticus) {
            delete interactGallusDomesticus;
        }

        Vector2 touchPosition = inputHandler.GetTouchPosition();
        interactGallusDomesticus = new GallusDomesticus{
            touchPosition.x, touchPosition.y, 
            0.0f, 0.0f, 
            20.0f, 
            BLUE, 
            State::Chicken, 
            0.0f
        };
    }

    // Check for collision between interactGallusDomesticus and other GallusDomesticus instances
    if (interactGallusDomesticus) {
        bool interactCollision = false;
        for (auto& gallus : gallusDomestici) {
            if (CheckCollisionCircles(
                {gallus.x, gallus.y}, gallus.radius,
                {interactGallusDomesticus->x, interactGallusDomesticus->y}, interactGallusDomesticus->radius)) {
                Vector2 direction = Vector2Normalize((Vector2){
                    gallus.x - interactGallusDomesticus->x,
                    gallus.y - interactGallusDomesticus->y
                });
                
                float speed = sqrtf(gallus.speedX * gallus.speedX + gallus.speedY * gallus.speedY);
                gallus.speedX = direction.x * speed * 1.5f; // Increase speed slightly
                gallus.speedY = direction.y * speed * 1.5f;

                interactCollision = true;
                break;
            }
        }
        
        // Add a timer to delete interactGallusDomesticus if it's not null
        static float interactTimer = 0.0f;
        interactTimer += deltaTime;
        if (interactTimer >= 0.1f) {
            delete interactGallusDomesticus;
            interactGallusDomesticus = nullptr;
            interactTimer = 0.0f;
        }

        (void)interactCollision;  // Silences the unused variable warning
    }

    // Debug output
    static int frameCount = 0;
    if (++frameCount % 60 == 0) {  // Print every 60 frames
        std::cout << "Current GallusDomesticus count: " << gallusDomestici.size() << std::endl;
    }
}

// 4.2 Update Individual GallusDomesticus
void Eggcken::UpdateGallusDomesticus(GallusDomesticus& gallus, float deltaTime) {
    gallus.age += deltaTime;

    switch (gallus.state) {
        case State::Egg:
            if (gallus.age >= 5.0f) {  // Hatch after 5 seconds
                // 1 in 5 chance of hatching into a Gallus
                if (GetRandomValue(0, 4) == 0) {
                    gallus.state = State::Gallus;
                    gallus.color = RED;
                } else {
                    gallus.state = State::Chicken;
                    gallus.color = WHITE;
                }
                gallus.radius = 10.0f;
                // Initialize speed for the newly hatched chicken/gallus
                float angle = GetRandomValue(0, 360) * DEG2RAD;
                float speed = 2.0f;
                gallus.speedX = speed * cosf(angle);
                gallus.speedY = speed * sinf(angle);
            }
            break;
        case State::Chicken:
        case State::Gallus:
            // Boundary checking code
            if (gallus.x - gallus.radius <= 0) {
                gallus.speedX = std::abs(gallus.speedX);
                gallus.x = gallus.radius;
            } else if (gallus.x + gallus.radius >= GetScreenWidth()) {
                gallus.speedX = -std::abs(gallus.speedX);
                gallus.x = GetScreenWidth() - gallus.radius;
            }

            if (gallus.y - gallus.radius <= 0) {
                gallus.speedY = std::abs(gallus.speedY);
                gallus.y = gallus.radius;
            } else if (gallus.y + gallus.radius >= GetScreenHeight()) {
                gallus.speedY = -std::abs(gallus.speedY);
                gallus.y = GetScreenHeight() - gallus.radius;
            }

            gallus.update();
            break;
    }
}

// Section 5: Egg Creation and Removal
// 5.1 Create Egg
void Eggcken::CreateEgg(const Vector2& position) {
    if (gallusDomestici.size() >= MAX_GALLUSDOMESTICUS) {
        std::cout << "Max GallusDomesticus reached. Removing oldest..." << std::endl;
        RemoveOldestGallusDomesticus();
    }

    GallusDomesticus egg = {
        position.x, position.y,
        0.0f, 0.0f,  // Eggs don't move
        5.0f,  // Smaller radius for eggs
        YELLOW,  // Egg color
        State::Egg,
        0.0f  // Initial age
    };
    gallusDomestici.push_back(egg);
    
    std::cout << "Created new Egg. Total count: " << gallusDomestici.size() << std::endl;
}

// 5.2 Remove Oldest GallusDomesticus
void Eggcken::RemoveOldestGallusDomesticus() {
    if (!gallusDomestici.empty()) {
        auto oldest = std::max_element(gallusDomestici.begin(), gallusDomestici.end(),
            [](const GallusDomesticus& a, const GallusDomesticus& b) {
                return a.age < b.age;
            });
        
        if (oldest != gallusDomestici.end()) {
            std::cout << "Removing GallusDomesticus with age: " << oldest->age << std::endl;
            gallusDomestici.erase(oldest);
        } else {
            std::cout << "Error: Could not find oldest GallusDomesticus" << std::endl;
        }
    } else {
        std::cout << "No GallusDomesticus to remove" << std::endl;
    }
    std::cout << "Current GallusDomesticus count: " << gallusDomestici.size() << std::endl;
}

// Section 6: Drawing
// 6.1 Draw Method
void Eggcken::Draw() {
    for (const auto& gallus : gallusDomestici) {
        gallus.draw();
    }
    if (interactGallusDomesticus) {
        interactGallusDomesticus->draw();
    }
}

// Section 7: Getter Methods
// 7.1 Get GallusDomesticus Positions
std::vector<Vector2> Eggcken::GetGallusDomesticusPositions() const {
    std::vector<Vector2> positions;
    for (const auto& gallus : gallusDomestici) {
        positions.push_back((Vector2){gallus.x, gallus.y});
    }
    return positions;
}

// 7.2 Get GallusDomesticus Radii
std::vector<float> Eggcken::GetGallusDomesticusRadii() const {
    std::vector<float> radii;
    for (const auto& gallus : gallusDomestici) {
        radii.push_back(gallus.radius);
    }
    return radii;
}

// 7.3 Get Interact GallusDomesticus Position
Vector2 Eggcken::GetInteractGallusDomesticusPosition() const {
    if (interactGallusDomesticus) {
        return { interactGallusDomesticus->x, interactGallusDomesticus->y };
    }
    return { 0, 0 };
}

// 7.4 Get Interact GallusDomesticus Radius
float Eggcken::GetInteractGallusDomesticusRadius() const {
    if (interactGallusDomesticus) {
        return interactGallusDomesticus->radius;
    }
    return 0;
}

// Section 8: Utility Functions
// 8.1 Get Random Value (if not already provided by raylib)
int Eggcken::GetRandomValue(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

// 8.2 Vector2 Normalize (if not already provided by raylib)
Vector2 Eggcken::Vector2Normalize(Vector2 v) {
    float length = std::sqrt(v.x * v.x + v.y * v.y);
    if (length > 0) {
        v.x /= length;
        v.y /= length;
    }
    return v;
}

// Section 9: Debug Functions
// 9.1 Print GallusDomesticus Info
void Eggcken::PrintGallusDomesticusInfo() const {
    std::cout << "GallusDomesticus Info:" << std::endl;
    for (size_t i = 0; i < gallusDomestici.size(); ++i) {
        const auto& gallus = gallusDomestici[i];
        std::cout << "GallusDomesticus " << i << ": "
                  << "State: " << static_cast<int>(gallus.state) << ", "
                  << "Position: (" << gallus.x << ", " << gallus.y << "), "
                  << "Speed: (" << gallus.speedX << ", " << gallus.speedY << "), "
                  << "Age: " << gallus.age << std::endl;
    }
}

// End of Eggcken.cpp