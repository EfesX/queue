#include <assert.h>
#include "equeue/queue.hpp"

using namespace efesx::queue;

int main(){
    queue q;

    q.enqueue(0, 78);

    assert(q.dequeue()->sint32_value() == 78);

    return 0;
}