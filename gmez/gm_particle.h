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
    void MoveLeftDown(int step = 1)
    {
        x = x - step;
        y = y + step;
    }

    void MoveRightDown(int step = 1)
    {
        x = x + step;
        y = y + step;
    }

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
            MoveLeftDown();
            state = 1;
        }
        // Move right = end
        else if (!neighbor[7])
        {
            MoveRightDown();
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


struct PIXEL
{
public:
    PIXEL(int _x, int _y) 
        :x(_x), y(_y) { }
public:
    int x;
    int y;
};

class wall_piel
{
private:
    int x;
    int y;
    int radius;

    vector<PIXEL> data;

public:
    wall_piel(int r = 0): radius(r)
    {
    }

    // Algorithm : point is inside the circel = (x, y ,r);
    // (x - xc)^2 + (y - yc)^2 < r^2
    bool check_point_in_circle(int x , int y , int r, // circle infor
                               int xc, int yc)        // point check
    {
        return std::pow(x - xc, 2) + std::pow(y - yc, 2) < std::pow(r, 2);
    }

    vector<PIXEL> neighbor_pixel(int x, int y)
    {
        vector<PIXEL> radius_offset_map;

        if (radius <= 0)
        {
            radius_offset_map.push_back(PIXEL(x, y));
            return radius_offset_map;
        }

        // Check the point of the square is inside the circle;
        for (int i = -radius; i < radius; i++)
        {
            for (int j = -radius; j < radius; j++)
            {
                if (check_point_in_circle(0, 0, radius, i, j))
                {
                    radius_offset_map.push_back(PIXEL(i, j));
                }
            }
        }

        // Actual coordinates after subtracting offset
        for (int i = 0; i < radius_offset_map.size(); i++)
        {
            radius_offset_map[i].x += x;
            radius_offset_map[i].y += y;
        }
        return radius_offset_map;
    }

    void Add(int x, int y)
    {
        data.push_back(PIXEL(x, y));
    }

    int SizePixel()
    {
        return data.size();
    }

    void GetXY(int i, int& x, int& y)
    {
        x = data[i].x;
        y = data[i].y;
    }
};