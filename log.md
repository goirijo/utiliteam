# The Log

This is where we keep track of what we've been talking about during each meeting.
It's written in Markdown, which you can learn how to use [here](https://www.markdownguide.org/basic-syntax/).
We can have a section added each week.
No ugly posts allowed.

## Setting up
### 05-Feb-2020, JGG
I'm not totally sure what we'll end up adding to this repo, but I think having someone log what we do will at least be good practice for using git.
Projects we come up with can either be developed directly here or included as a submodule.
We can also share different resources that might be useful for the group.

----
## Data structures
### 07-Feb-2020, EGP

#### [Linked_lists](slides/Linked_C%2B%2B.pdf) \(presented by Farnaz\) 
A linked list is a type of linear data structure that stores data.
A linked list contains nodes, which are made of two parts.
One part contains the data and the second is a pointer that points to the address of the following node.
Since nodes contain pointers that point to the next node in the linked list, these lists do not store data in contiguous memory locations, like arrays and vectors do.
Due to this feature, it is easier and faster to add items at the beginning or middle of a list.

Advantages of using linked lists:
- Dynamic size
- Ease of insertion/deletion at different parts of the list

Disadvantages of using linked lists:
- Random cases are not allowed
- Extra memory space is used for pointers
- Not cache friendly

Useful links:
- <https://www.geeksforgeeks.org/linked-list-set-1-introduction/>
- <http://www.cplusplus.com/articles/LACRko23/>
- <https://www.softwaretestinghelp.com/linked-list/>

#### Stacks \(presented by Farnaz\)
Stacks were not disscussed in much detail, but here are a few features about stacks:
- Stacks work with the *Last In First Out* mechanism, where data is added and removed from one end of the container only.
- At any given point, the user only has access to the "top" of the container.

Useful links:
- <https://www.geeksforgeeks.org/stack-in-cpp-stl/>
- <http://www.cplusplus.com/reference/stack/stack/>
- <https://www.geeksforgeeks.org/stack-push-and-pop-in-c-stl/>

#### Ordered and Unordered Maps \(presented by Muna\)
Maps are associative containers which store data in a combination of *key values* and *mapped values*.
*Key values* sort and identify the data, therefore keys must be unique; in a particular map, two identical keys cannot map different values.

In ordered maps:
- Data is organized by increasing order; a *less than* operator is required
- Data is stored using a binary tree
- The time scale for searching, inserting, or deleting is logarthmic time scale

In unordered maps:
- There is no ordereding of the different keys, therefore only an *equals* operator is required
- Data is processed by a hash function and stored in a hash table
- Search, insertion, and deletion time is much quicker than in ordered maps

Useful links:
- <http://www.cplusplus.com/reference/map/map/>
- <https://www.geeksforgeeks.org/map-vs-unordered_map-c/>
- <https://en.cppreference.com/w/cpp/container/map> 
