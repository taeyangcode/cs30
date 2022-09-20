## Project 1: Just Got Married!

### Prompt

Do you want to know how challenging it was to get married last year during this
pandemic? Challenging to say the least. Forget picking the place for the wedding
ceremony, the venue for the wedding reception, and the food. The one thing they never
tell you is that the hardest part of the wedding planning is actually the guest list. Who
do you invite? Who do you say no too? How many people? Believe it or not, it’s a lot
of politics and family drama, if you’re not careful. So a word to the wise, which is to
remember that it’s your wedding. =]
To help me recreate this unfortunate act in the wedding planning, you will write the
implementation of the WeddingGuest class using a doubly linked list, which should be
sorted alphabetically according to last name, then first name. You will also implement a
couple of algorithms that operate on the WeddingGuest class.

### Approach

#### Grading Scale

| Data Structure              | Possible Points                    |
| --------------------------- | ---------------------------------- |
| Dynamically resizable array | 126 (out of 140)                   |
| Linked list (singly)        | 140                                |
| Linked list (doubly)        | 147 (up to 7 extra credit points)  |
| Binary tree                 | 147 (up to 7 extra credit points)  |
| Hash table                  | 154 (up to 14 extra credit points) |

As the grading scale for this project differs by the data structure the student chooses, I decided to implement a chained hash table of doubly linked lists for the potential of maximum extra credit. A requirement of the data structure also states that guests should be ordered, and with no option for an ordered map data structure, I decided to implement a sorting hash function that may disagree with what others may consider "efficient."
The sorting/inserting requirements of nodes are as follows:

> If the full name (both the first and last name) is not equal to any full name currently in the list then add it. Elements should be added according to their last name. Elements with the same last name should be added according to their first names. Otherwise, make no change to the list.

_Note: The rubric also mentions that empty strings are valid inputs for either a guest's first or last name, though no information regarding special characters._

#### Implementation

The algorithm of the hashing function relies on the idea that there exists 27 buckets within the hash table, one for each case-insensitive letter of the alphabet as well as an extra bucket for special characters (any non-alphabetic character, whitespace, zero-width space, etc.). Using this logic the hashing function accepts a guest's last name as the key, and utilizes the ascii code of the first letter of the key (if the value is not empty) to determine its bucket index. In the case that the first letter of the key is uppercase, subtract 65 (lowercase a ascii), if lowercase, subtract 97 (uppercase A ascii), to return an integer [0, 25]. In all other cases, index 26 should be returned.

```
// Logic in pseudocode
function hash(lastName)
    if lastName is empty then
        return 26
    firstLetter < lastName[0]
    if firstLetter is an uppercase letter
        return firstLetter - 65
    if firstLetter is a lowercase letter
        return firstLetter - 97
    return 26
```

Inserting and locating elements simply uses the hash function to determine the head node at the bucket index, then cycling through the nodes comparing the the last name and first name values as needed.

### License

All source code is listed under an MIT license.
By this token anyone may feel free to "use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software" under the condition that the license text located in `LICENSE.md` "shall be included in all copies or substantial portions of the Software."

### Contact

Github: https://github.com/taeyangcode
Email: taeyangcode@gmail.com
Discord: 태양#0001
