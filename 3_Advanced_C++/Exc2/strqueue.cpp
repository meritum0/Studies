#include "strqueue.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <deque>
#include <iterator>
#include <cassert>
#include <climits>

#ifdef NDEBUG
    constexpr static bool debug = false;
#else
    constexpr static bool debug = true;
#endif

#define LOG_MSG(msg)                                                           \
    do                                                                         \
    {                                                                          \
        if constexpr (debug)                                                   \
            std::cerr << __func__ << msg << std::endl;                         \
    } while (false)

using queue = std::deque<std::string>;
using queue_map = std::unordered_map<unsigned long, queue>;
using queue_iter = queue::iterator;

queue_map &get_queues() 
{
    static queue_map queues;
    return queues;
}

namespace cxx
{
    unsigned long strqueue_new() 
    {
        LOG_MSG("()");
            
        static unsigned long identifier = 0;

        assert(identifier != ULONG_MAX);

        get_queues()[identifier] = queue();

        LOG_MSG(" returns " << identifier);
        return identifier++;
    }

    void strqueue_delete(unsigned long id) 
    {
        LOG_MSG('(' << id << ')');

        if (get_queues().erase(id))
            LOG_MSG(" done");
        else 
            LOG_MSG(": queue " << id << " does not exist");
    }

    size_t strqueue_size(unsigned long id) 
    {
        LOG_MSG('(' << id << ')');

        auto search_res = get_queues().find(id);

        size_t res = 0;
        if (search_res != get_queues().end())
            res = search_res->second.size();
        else
            LOG_MSG(": queue " << id << " does not exist");
        
        LOG_MSG(" returns " << res);
        return res;
    }

    void strqueue_insert_at(unsigned long id, size_t position, const char* str)
    {
        LOG_MSG('(' << id <<  ", " << position << ", "    << (str ? std::string("\"") + str + '"' : "NULL") << ')');

        auto search_res = get_queues().find(id);

        if (str == NULL)
        {
            LOG_MSG(" failed");
        }
        else if (search_res == get_queues().end())
        {
            LOG_MSG(": queue " << id << " does not exist");
        }
        else
        {
            queue &deq = search_res->second;

            deq.insert(position < deq.size() ?  deq.begin() + position : deq.end(), str);

            LOG_MSG(" done");
        }
    }

    void strqueue_remove_at(unsigned long id, size_t position) 
    {
        LOG_MSG('(' << id <<  ", " << position << ')');
        
        auto search_res = get_queues().find(id);
        if (search_res != get_queues().end() && position < search_res->second.size())
        {
            queue &deq = search_res->second;
            queue_iter pos_it = deq.begin() + position;
            deq.erase(pos_it);
            
            LOG_MSG(" done");
        }
        else
        {
            if (search_res == get_queues().end())
                LOG_MSG(": queue " << id << " does not exist");
            else
                LOG_MSG(": queue " << id << " does not contain string at position " << position);
        }
    }
    
    const char* strqueue_get_at(unsigned long id, size_t position)
    {
        LOG_MSG('(' << id <<  ", " << position << ')');
        
        auto search_res = get_queues().find(id);
        if (search_res != get_queues().end() && position < search_res->second.size())
        {
            queue &deq = search_res->second;
            
            LOG_MSG(" returns \"" << deq.at(position) << '"');
            return deq.at(position).c_str();
        }
        else
        {
            if (search_res == get_queues().end())
                LOG_MSG(": queue " << id << " does not exist");
            else
                LOG_MSG(": queue " << id << " does not contain string at position " << position);
            
            LOG_MSG(" returns NULL");
            return nullptr;
        }
    }
    
    void strqueue_clear(unsigned long id)
    {
        LOG_MSG('(' << id <<  ')');
        
        auto search_res = get_queues().find(id);
        if (search_res != get_queues().end())
        {
            search_res->second.clear();
            LOG_MSG(" done");
        }
        else
        {
            LOG_MSG(": queue " << id << " does not exist");
        }
    }
    
    int strqueue_comp(unsigned long id1, unsigned long id2)
    {
        LOG_MSG('(' << id1 <<  ", " << id2 << ')');
        
        int res = 0;
        
        auto search_res1 = get_queues().find(id1);
        auto search_res2 = get_queues().find(id2);
        
        queue deq1, deq2;

        if (search_res1 != get_queues().end())
            deq1 = search_res1->second;
        else
            LOG_MSG(": queue " << id1 << " does not exist");

        if (search_res2 != get_queues().end())
            deq2 = search_res2->second;
        else
            LOG_MSG(": queue " << id2 << " does not exist");
        
        auto iter1 = deq1.begin();
        auto iter2 = deq2.begin();

        while (iter1 != deq1.end() && iter2 != deq2.end())
        {
            if (*iter1 != *iter2)
                break;

            iter1++;
            iter2++;
        }
        
        if (iter1 != deq1.end() && iter2 != deq2.end())
        {
            if (*iter1 < *iter2)
                res = -1;
            else    //  (*iter1 > *iter2)
                res = 1;
        }
        else
        {
            if (iter1 == deq1.end() && iter2 != deq2.end())
                res = -1;
            else if (iter1 != deq1.end() && iter2 == deq2.end())
                res = 1;
            //  else res remains 0; queues are equal.
        }

        LOG_MSG(" returns " << res);
        return res;
    }
} // namespace cxx
