# Docs

Welcome to docs.

## A.1 Arcitecture

### A.1.1 Elevator

```mermaid
classDiagram
    class Elevator {
        -State state
        -Queue floor_queue
        -Timer timer
        -int current_floor
        -bool obstructed

        +Elevator(floor_number)
        +order(floor_number, button_type)
        +stop()
        +update_floor(floor)
        +set_obstructed(obstructed)
    }

    class State {
        <<enumeration>>
        GOING_UP
        DOORS_OPEN
        STOPPED
        GOING_DOWN
    }

    class ButtonType {
        <<enumeration>>
        HALL_UP
        HALL_DOWN
        CABIN
    }
```

### A.1.2 Hardware Reader

```mermaid
classDiagram
    class HardwareReader {
        -Elevator elevator
        +loop()
    }
```

### A.1.3 Communication

```mermaid
sequenceDiagram
    participant E as Elevator
    participant HR as Hardware Reader
    participant H as Hardware

    loop
        HR->>H: Read floor sensor
        H->>HR: Return floor number
        opt sensor activated
            HR->>E: update_floor
        end

        HR->>H: Read stop button
        H->>HR: Return should stop

        HR->>E: setStopped

        HR->>H: Read obstruction sensor
        H->>HR: Return is obstructed
        HR->>E: set obstructed

        loop for each button
            HR->>H: Read order button
            H->>HR: Return is pressed
            opt is pressed
                HR->>E: order
            end
        end
    end

```


## A.2 Modules

### A.2.1 State Machine

```mermaid
stateDiagram-v2
    state fork_state <<choice>>
    [*] --> fork_state

    fork_state --> Unknown : invalid floor
    fork_state --> DoorsClosed : vaild floor

    Unknown --> DoorsClosed : update_floor

    

    DoorsClosed --> GoingDown : order or timer
    DoorsClosed --> GoingUp : order or timer
    DoorsClosed --> DoorsOpen : order

    GoingUp --> DoorsOpen : update_floor
    GoingDown --> DoorsOpen : update_floor
    DoorsOpen --> DoorsClosed : timer
    DoorsOpen --> GoingDown : timer
    DoorsOpen --> GoingUp : timer


```

```python

reader = HardwareReader()
floor = reader.read_floor()
elevator = Elevator(floor)

while True:
    reader.loop()

```

```mermaid
stateDiagram-v2
    Created --> PendingPayment : Place Order
    PendingPayment --> Paid : Payment Successful
    PendingPayment --> Cancelled : Payment Failed

    Paid --> Processing : Confirm Order
    Processing --> Shipped : Ship Order
    Shipped --> Delivered : Confirm Delivery

    Delivered --> [*]
    Cancelled --> [*]

    state Processing {
        [*] --> Packing
        Packing --> QualityCheck
        QualityCheck --> ReadyForShipment
        ReadyForShipment --> [*]
    }
```