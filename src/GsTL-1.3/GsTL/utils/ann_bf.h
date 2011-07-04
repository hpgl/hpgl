

#include <string>


struct one_dimension_tag {};
struct high_dimension_tag {};


template <typename container>
struct container_traits
{
        typedef high_dimension_tag container_dimension_category;
        typedef typename container::value_type value_type;
};

template <>
struct container_traits<float>
{
        typedef one_dimension_tag container_dimension_category;
        typedef float value_type;
};

template <>
struct container_traits<const float>
{
        typedef one_dimension_tag container_dimension_category;
        typedef float value_type;
};

template <>
struct container_traits<double>
{
        typedef one_dimension_tag container_dimension_category;
        typedef double value_type;
};

template <>
struct container_traits<const double>
{
        typedef one_dimension_tag container_dimension_category;
        typedef double value_type;
};

template <>
struct container_traits<long double>
{
        typedef one_dimension_tag container_dimension_category;
        typedef long double value_type;
};

template <>
struct container_traits<const long double>
{
        typedef one_dimension_tag container_dimension_category;
        typedef long double value_type;
};

template <>
struct container_traits<int>
{
        typedef one_dimension_tag container_dimension_category;
        typedef int value_type;
};

template <>
struct container_traits<const int>
{
        typedef one_dimension_tag container_dimension_category;
        typedef int value_type;
};

template <>
struct container_traits<string>
{
        typedef one_dimension_tag container_dimension_category;
        typedef string value_type;
};

template <>
struct container_traits<const string>
{
        typedef one_dimension_tag container_dimension_category;
        typedef string value_type;
};


template <typename Forward_Iterator, typename T, 
typename BinaryFunction>
Forward_Iterator nearest_neighbor(Forward_Iterator first, 
Forward_Iterator last, T point, BinaryFunction distance)
{
typename BinaryFunction::result_type min, temp;
Forward_Iterator min_it;

min = distance(*first, point,
container_traits<iterator_traits<Forward_Iterator>::\
value_type>::container_dimension_category(),
container_traits<T>::container_dimension_category());
        
min_it = first;

++first;

while (first != last)
{
        temp = distance(*first, point,
        container_traits<iterator_traits<Forward_Iterator>::\
        value_type>::container_dimension_category(), 
        container_traits<T>::container_dimension_category());

        if (temp < min)
        {
                min_it = first;
                min = temp;
        }
        ++first;
}

return min_it;
}


template <typename Element, typename Point>
struct euclidean_distance
{
typedef typename container_traits<Element>::value_type R;
typedef R result_type;

R operator()(Element& element, Point& point, one_dimension_tag, 
        one_dimension_tag) const
{
        if (point < element) 
                return (R)(element - point);
        else 
                return (R)(point - element);
}
R operator()(Element& element, Point& point, high_dimension_tag, 
        high_dimension_tag) const
{
        typename Element::iterator x_start  = element.begin();
        typename Element::iterator x_end = element.end();
        typename Point::iterator y_start = point.begin();
        typename Point::iterator y_end = point.end();
  
        R distance = (R)0;

        while(x_start != x_end && y_start != y_end)
        {
           distance +=(*x_start-*y_start)*(*x_start - *y_start);
           ++x_start;
           ++y_start;
        }
        return distance;
        }
};


template <typename Element, typename Point>
struct manhattan_distance
{
typedef typename container_traits<Element>::value_type R;
typedef R result_type;

R operator()(Element& element, Point& point, one_dimension_tag, 
        one_dimension_tag) const
{
        if (point < element) 
                return (R)(element - point);
        else 
                return (R)(point - element);
}
R operator()(Element& element, Point& point, high_dimension_tag, 
        high_dimension_tag) const
{
        typename Element::iterator x_start  = element.begin();
        typename Element::iterator x_end = element.end();
        typename Point::iterator y_start = point.begin();
        typename Point::iterator y_end = point.end();
  
        R distance = (R)0;

        while(x_start != x_end && y_start != y_end)
        {
                if (*x_start > *y_start)
                        distance += (*x_start - *y_start);
                else
                        distance += (*y_start - *x_start);
                ++x_start;
                ++y_start;
        }
        return distance;
}
};


template <typename Element, typename Point>
struct max_distance
{
typedef typename container_traits<Element>::value_type R;
typedef R result_type;

R operator()(Element& element, Point& point, one_dimension_tag, 
        one_dimension_tag) const
{
        if (point < element) 
                return (R)(element - point);
        else 
                return (R)(point - element);
}
R operator()(Element& element, Point& point, high_dimension_tag, 
        high_dimension_tag) const
{
        typename Element::iterator x_start  = element.begin();
        typename Element::iterator x_end = element.end();
        typename Point::iterator y_start = point.begin();
        typename Point::iterator y_end = point.end();

        R distance = (R)0;
        R temp_distance;

        while(x_start != x_end && y_start != y_end)
        {
                if (*x_start > *y_start)
                        temp_distance = *x_start - *y_start;
                else
                        temp_distance = *y_start - *x_start;

                if (distance < temp_distance)
                        distance = temp_distance;

                ++x_start;
                ++y_start;
        }
        return distance;
}
};

