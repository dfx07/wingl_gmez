////////////////////////////////////////////////////////////////////////////////////
// File: gm_map.h   -- Map data game engine                                         
// Copyright (C) 2021-2022 Thuong.NV   Created : 13/08/2022                         
// For conditions of distribution and use, see copyright notice in readme.txt       
////////////////////////////////////////////////////////////////////////////////////


class particel_piel
{
public:
    int x;
    int y;
public:
    virtual void GetXY(int& x, int& y)
    {
        x = this->x;
        y = this->y;
    }

public:
    virtual void Update()
    {

    }


};


class sand_piel : public particel_piel
{
public:
    sand_piel()
    {
        x = 0;
        y = 0;
    }

    sand_piel(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

private:
    void MoveLeft(int step = 1)
    {
        x = x - step;
    }

    void MoveRight(int step = 1)
    {
        x = x + step;
    }

    void MoveDown(int step = 1)
    {
        y = y + step;
    }
public:
    virtual int Update(int neighbor[8])
    {
        // Data struct neighbor
        //[0][1][2]
        //[3][x][4]
        //[5][6][7]
        int state = 0; // 1 : move | 0 stop

        // Move Fall
        if (!neighbor[6])
        {
            MoveDown();
            state = 1;
        }
        // Move left prioritized
        else if (!neighbor[5])
        {
            MoveLeft();
            state = 1;
        }
        // Move right = end
        else if (!neighbor[7])
        {
            MoveRight();
            state = 1;
        }

        return state;
    }
};

class water_piel : public particel_piel
{
private:
    int old_move = 0;

public:
    int color_index = 0;

    float blue = 0;

public:
    water_piel()
    {
        x = 0;
        y = 0;
    }

    water_piel(int x, int y)
    {
        this->blue = rand()% 1;
        this->x = x;
        this->y = y;
    }

private:
    void MoveLeft(int step = 1)
    {
        old_move = 2;
        x = x - step;
    }

    void MoveRight(int step = 1)
    {
        old_move = 1;
        x = x + step;
    }

    void MoveDown(int step = 1)
    {
        old_move = 0;
        y = y + step;
    }

public:
    virtual int Update(int neighbor[8])
    {
        // Data struct neighbor
        //[0][1][2]
        //[3][x][4]
        //[5][6][7]
        int state = 0; // 1 : move | 0 stop

        // Move Fall
        if (!neighbor[6])
        {
            MoveDown();
            state = 1;
        }
        else if (old_move == 0)
        {
            if (!neighbor[3])
            {
                MoveLeft();
                state = 1;
            }
            else if (!neighbor[4])
            {
                MoveRight();
                state = 1;
            }
        }
        // Move left prioritized
        else if (old_move == 2)
        {
            if (!neighbor[3])
            {
                MoveLeft();
                state = 1;
            }
            else if (!neighbor[4])
            {
                MoveRight();
                state = 1;
            }
        }
        // Move right = end
        else if (old_move == 1)
        {
            if (!neighbor[4])
            {
                MoveRight();
                state = 1;
            }
            else if (!neighbor[3])
            {
                MoveLeft();
                state = 1;
            }
        }

        return state;
    }
};