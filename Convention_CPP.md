# C++ Coding Conventions

These conventions are inspired by **Clean Code** and **Clean Architecture** (Robert C. Martin) and are adapted for embedded C++ development on the Raspberry Pi Pico.

---

## 1. Naming

| Element | Convention | Example |
|---------|-----------|---------|
| Files | `snake_case` | `motion_sensor.hpp` |
| Classes / Structs | `PascalCase` | `MotionSensor` |
| usecase|  `PascalCase` + '_UC' | `TurnOnLight_UC` 
| Interfaces (pure abstract) | `I` prefix + `PascalCase` | `IMotionSensor` |
| Functions | `snake_case` | `orchestration_of_the_element()` |
| Methods  | `camelCase` | `isMotionDetected()` |
| private Methods  |  `_` prefix + `camelCase` | `_readSensor()` |
| Member variables | `_` prefix + `camelCase` | `_lastTriggerTime` |
| Constants / `constexpr` | `UPPER_SNAKE_CASE` | `DEBOUNCE_DELAY_MS` |
| Namespaces | `snake_case` | `domain`, `use_cases` |

**Rules:**
- Names must be **self-explanatory** – avoid abbreviations (`motionDetected`, not `md`).
- Boolean names should read as a question: `isOn()`, `hasExpired()`, `isNightTime()`.
- Avoid generic names like `data`, `value`, `temp` unless the scope is trivially small.
- Avoid comment as much as possible. Clean code does not need it.

---

## 2. Classes

- Keep classes **small and focused** – a class should have one clear reason to change.
- Prefer **composition over inheritance**.
- Limit the number of public methods; hide implementation details.
- Constructors should leave the object in a valid, ready-to-use state.
- Avoid default constructors when meaningful initialisation is required.
- public method should be extremely clear and abstract.

---

## 3. Single Responsibility Principle (SRP)

Every class, function, and module must have **exactly one responsibility**:

- A sensor driver reads raw hardware values – it does not interpret them.
- A domain entity encodes business rules – it has no knowledge of hardware.
- A use case orchestrates entities and drivers – it contains no business logic itself.

If you find yourself writing `// Part 1 …` and `// Part 2 …` inside a single function, split the function.

---

## 4. Dependency Inversion Principle (DIP)

- High-level modules (domain, use cases) must **not depend on** low-level modules (drivers, hardware).
- Both must depend on **abstractions** (interfaces defined in `3_interfaces/`).
- Concrete hardware implementations are injected at construction time (constructor injection).

```cpp
// Good – use case depends on the abstraction, not the concrete driver
class AutoLightUseCase {
public:
    explicit AutoLightUseCase(ILightSensor& light, IMotionSensor& motion, ILight& lamp);
};
```

---

## 5. Interface-Based Design for Hardware

- Every hardware peripheral is accessed through a **pure abstract interface** declared in `3_interfaces/`.
- The interface expresses *what* is needed (not *how* it is done).
- Hardware-specific implementations live in `5_drivers/` or `4_infrastructure/`.
- This makes it trivial to substitute a fake/mock during unit testing.

```cpp
// src/3_interfaces/IMotionSensor.hpp
class IMotionSensor {
public:
    virtual ~IMotionSensor() = default;
    virtual bool isMotionDetected() const = 0;
};
```

---

## 6. Testable Logic Separated from Hardware

- Business rules and use-case logic must be **pure C++** with no direct calls to SDK functions (`gpio_get`, `sleep_ms`, …).
- Hardware interaction is always hidden behind an interface.
- Unit tests instantiate domain/use-case classes with mock implementations of the interfaces.
- Hardware integration tests run on the Pico via the host test runner.

---

## 7. Functions

- Functions should do **one thing** and do it well.
- Functions must be short: **maximum 15 lines** per function.
- Avoid output parameters; return values or use small result structs instead.
- Mark functions that do not modify state as `const`.
- Use `[[nodiscard]]` on functions whose return value must be checked.

### 7.1 Single Level of Abstraction in Entry-Point Functions

Exemple :

void execute()
{
    updateMotion();
    evaluateLightingConditions();
    applyLightDecision();
}

## 8. Formatting
Tabs (4 spaces)
max 100 chars / ligne
toujours {}

## 9. Key Principle

👉 Lire une méthode publique = comprendre le comportement sans lire les détails

👉 Lire _functions = comprendre comment ça marche
