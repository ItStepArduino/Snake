// The pins of the led board rows
byte Rows[8] { 2, 3, 4, 5, 6, 7, 8, 9 };
// The pins of the led board collumns
byte Collumns[8] { 10, 11, 12, 13, A0, A1, A2, A3 };

// The pins for the joystick x and y axis
byte JoyX = A5, JoyY = A4;

// The time (in milliseonds) to wait before the snake moves again
#define WaitingTime 700

// Has the snake stopped moving?
bool Pause = false;


// A simple point structure
struct Point
{
    int x, y;
    Point(int _x, int _y)
    {
        x = _x;
        y = _y;
    }

    // Are the given x and y parameters equal to the points x and y parameters
    bool Is(int _x, int _y)
    {
        return x == _x && y == _y;
    }
};

// The apple the snake is going for
Point Apple = Point(2, 2);

// The direction the snake is moving in
enum Direction
{
    Up,
    Left,
    Down,
    Right
};

// The main snake class
class _Snake
{
private:
    // The structure representing every cell of the snake
    struct Node
    {
        // The cell's position
        Point Pos = Point(-1, -1);
        // The next and previous cell relative to this cell
        Node *Next = nullptr, *Prev = nullptr;
        // An empty initializer
        Node()
        {
            
        }
    };

    // The memory block containing all the cells
    Node Mem[64] { Node() };
    // The head and tail nodes of the snake (pointers)
    Node *Head = 0, *Tail = 0;
    // The number of cells in the snake (the length of the snake)
    int Count = 1;
public:
    // Initialize a snake with the given starting length
    _Snake(int Len)
    {
        // Create the snake cell
        Node *n = &Mem[0];
        n->Pos = Point(6, 6);
        Head = n;
        Tail = n;
        // Create all the other snake cells
        for (int i = 1; i < Len; i++)
        {
            Add(); // Create the cell
            Move(Direction::Up); // Move the cell
        }
    }

    // Add a new snake cell
    void Add()
    {
        // Create a new snake cell from the predefined memory block
        Node *n = &Mem[Count++];
        n->Pos = Head->Pos;
        Tail->Next = n;
        n->Prev = Tail;
        Tail = n;
    }

    // Move the snake in the given direction
    void Move(Direction Dir)
    {
        // Extract the position in which we're moving the snake
        Point New = Point(-1, -1);
        int x = Head->Pos.x, y = Head->Pos.y;
        switch (Dir)
        {
            case Direction::Up:
                New = Point(x, y - 1);
                break;
            case Direction::Left:
                New = Point(x - 1, y);
                break;
            case Direction::Down:
                New = Point(x, y + 1);
                break;
            case Direction::Right:
                New = Point(x + 1, y);
                break;
        }

        // Check if this is a valid position for the snake
        if (New.x < 0 || New.x > 7 || New.y < 0 || New.y > 7 || Has(New.x, New.y))
        {
            // If not, end the game
            Pause = true;
            return;
        }

        // Move the tail to the head position using the newly extracted position
        Node *n = Tail;
        Tail = n->Prev;
        Tail->Next = nullptr;
        Head->Prev = n;
        Head = n;
        n->Pos = New;

        // Check if we have hit an apple
        if (Apple.Is(Head->Pos.x, Head->Pos.y))
        {
            // We have filled the screen - o more places to move
            if (Count == 63)
            {
                // You can't go any further
                Pause = true;
                return;
            }

            // Add a snake cell
            Add();
            // Not the best method to find a new position for the apple, but it's highly unlikely that a player will have a long enough snake for this operation to take a long time
            while (Has(Apple.x, Apple.y))
            {
                Apple.x = random(0, 8);
                Apple.y = random(0, 8);
            }
        }
    }

    // Access the snake cell at the given index
    Point operator[](int Index)
    {
        // Iterate through the cells untill we have the cell we are looking for
        Node *n = Head;
        for (int i = 0; i < Index; i++)
            n = n->Next;

        return n->Pos;
    }

    // Does the snake contain the given point?
    bool Has(int x, int y)
    {
        // Go through all the cells and check if we have a cell that matches
        for (int i = 0; i < Count; i++)
            if (Mem[i].Pos.Is(x, y))
                // We found a cell
                return true;

        // No cells were found with the given position
        return false;
    }
};

// Define our snake
_Snake Snake = _Snake(3);

void setup()
{
    // Setup our rows and collumns for display
    for (int i = 0; i < 8; i++)
    {
        pinMode(Rows[i], OUTPUT);
        pinMode(Collumns[i], OUTPUT);
    }

    // Setup our joystick
    pinMode(JoyX, INPUT);
    pinMode(JoyY, INPUT);
}

// The direction the snkae is currently going in
Direction dir = Direction::Up;
// The last time the snake moved
unsigned long t = millis();
void loop()
{
    // Should the snake continue moving?
    if (!Pause)
    {
        // Get the joystick values where: value ∈ [0, 1]
        double jx = (double)(1023 - analogRead(JoyX)) / 1023,
               jy = (double)analogRead(JoyY) / 1023;

        // Decide if we should update the snake's moving direction
        if (jx <= 0.2)
            dir = Direction::Up;
        else if (jx >= 0.8)
            dir = Direction::Down;
        else if (jy <= 0.2)
            dir = Direction::Left;
        else if (jy >= 0.8)
            dir = Direction::Right;

        // Check if we should move the snkae
        unsigned long _t = millis();
        if (_t - t >= WaitingTime)
        {
            // Enough time has passed - move the snake
            t = _t;
            Snake.Move(dir);
        }
    }
    
    
    // Draw the snake & the apple
    for (int x = 0; x < 8; x++)
    {
        digitalWrite(Collumns[x], HIGH);
        for (int y = 0; y < 8; y++)
        {
            // Is there an apple or a snake cell in the given position?
            bool use = Apple.Is(y, 7 - x) || Snake.Has(y, 7 - x);
            digitalWrite(Rows[y], use ? LOW : HIGH);
            if (use)
            {
                // If the cell was on, make sure to turn it off before moving on
                delayMicroseconds(50);
                digitalWrite(Rows[y], HIGH);
            }
        }
        digitalWrite(Collumns[x], LOW);
    }
}
