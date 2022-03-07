#define LOG_LEVEL 2
#include "ctarget.h"
#define NUMBER_OF_FEDERATES 1
#define TARGET_FILES_DIRECTORY "/home/lfvmu/lf_flexpret/lf/Minimal/src-gen/Minimal"
#include "core/reactor.c"
#include "core/mixed_radix.h"
#include "../../../tinyalloc.h"
#include <flexpret_io.h>
// Code generated by the Lingua Franca compiler from:
// file://home/lfvmu/lf_flexpret/lf/Minimal/Minimal.lf
int main(int argc, char* argv[]) {
    _fp_print(1);
    extern char end; // Set by linker.
    // Allocate 128 bits (16 bytes) for the heap.
    ta_init(&end, &end+12800, 10, 16, 8);
    return lf_reactor_c_main(argc, argv);
}
// =============== START reactor class Minimal
typedef struct {
    struct self_base_t base;
    int s;
    reaction_t _lf__reaction_0;
    trigger_t _lf__t;
    reaction_t* _lf__t_reactions[1];
} minimal_self_t;
void minimalreaction_function_0(void* instance_args) {
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-variable"
    minimal_self_t* self = (minimal_self_t*)instance_args;
    ((self_base_t*)self)->executing_reaction = &self->_lf__reaction_0;
    
    #pragma GCC diagnostic pop
    /*Correspondence: Range: [(6, 8), (7, 4)) -> Range: [(0, 0), (1, 4)) (src=/home/lfvmu/lf_flexpret/lf/Minimal/Minimal.lf)*/
    // printf("Hello World. %d\n", self->s);
    _fp_print(1000);
        
}
minimal_self_t* new_Minimal() {
    minimal_self_t* self = (minimal_self_t*)_lf_new_reactor(sizeof(minimal_self_t));
    self->_lf__reaction_0.number = 0;
    self->_lf__reaction_0.function = minimalreaction_function_0;
    self->_lf__reaction_0.self = self;
    self->_lf__reaction_0.deadline_violation_handler = NULL;
    self->_lf__reaction_0.STP_handler = NULL;
    self->_lf__reaction_0.name = "?";
    self->_lf__t.last = NULL;
    self->_lf__t_reactions[0] = &self->_lf__reaction_0;
    self->_lf__t.reactions = &self->_lf__t_reactions[0];
    self->_lf__t.number_of_reactions = 1;
    self->_lf__t.is_timer = true;
    return self;
}
// =============== END reactor class Minimal

void _lf_set_default_command_line_options() {}
// Array of pointers to timer triggers to be scheduled in _lf_initialize_timers().
trigger_t* _lf_timer_triggers[1];
int _lf_timer_triggers_size = 1;
// Array of pointers to reactions to be scheduled in _lf_trigger_startup_reactions().
reaction_t** _lf_startup_reactions = NULL;
int _lf_startup_reactions_size = 0;
// Empty array of pointers to shutdown triggers.
reaction_t** _lf_shutdown_reactions = NULL;
int _lf_shutdown_reactions_size = 0;
trigger_t* _lf_action_for_port(int port_id) {
    return NULL;
}
void _lf_initialize_trigger_objects() {
    // Initialize the _lf_clock
    lf_initialize_clock();
    
    int _lf_startup_reactions_count = 0;
    int _lf_shutdown_reactions_count = 0;
    int _lf_timer_triggers_count = 0;
    int _lf_tokens_with_ref_count_count = 0;
    minimal_self_t* minimal_self[1];
    // ***** Start initializing Minimal of class Minimal
    minimal_self[0] = new_Minimal();
    { // For scoping
        static int _initial = 1;
        minimal_self[0]->s = _initial;
    } // End scoping.
    // Initializing timer Minimal.t.
    minimal_self[0]->_lf__t.offset = 0;
    minimal_self[0]->_lf__t.period = 0;
    _lf_timer_triggers[_lf_timer_triggers_count++] = &minimal_self[0]->_lf__t;
    //***** End initializing Minimal
    
    // **** Start deferred initialize for Minimal
    {
        // Total number of outputs (single ports and multiport channels)
        // produced by reaction_0 of Minimal.
        minimal_self[0]->_lf__reaction_0.num_outputs = 0;
        {
            int count = 0;
        }
        // ** End initialization for reaction 0 of Minimal
    }
    // **** End of deferred initialize for Minimal
    // **** Start non-nested deferred initialize for Minimal
    // Minimal.reaction_0 dominating upstream reaction.
    minimal_self[0]->_lf__reaction_0.last_enabling_reaction = NULL;
    // **** End of non-nested deferred initialize for Minimal
    // Connect inputs and outputs for reactor Minimal.
    
    
    // Set reaction priorities for ReactorInstance Minimal
    {
        minimal_self[0]->_lf__reaction_0.chain_id = 1;
        // index is the OR of level 0 and 
        // deadline 140737488355327 shifted left 16 bits.
        minimal_self[0]->_lf__reaction_0.index = 0x7fffffffffff0000LL;
    }
    
}
void _lf_trigger_startup_reactions() {
}
void _lf_initialize_timers() {
    for (int i = 0; i < _lf_timer_triggers_size; i++) {
        if (_lf_timer_triggers[i] != NULL) {
            _lf_initialize_timer(_lf_timer_triggers[i]);
        }
    }
}
void logical_tag_complete(tag_t tag_to_send) {
}
bool _lf_trigger_shutdown_reactions() {                          
    for (int i = 0; i < _lf_shutdown_reactions_size; i++) {
        if (_lf_shutdown_reactions[i] != NULL) {
            _lf_enqueue_reaction(_lf_shutdown_reactions[i]);
        }
    }
    // Return true if there are shutdown reactions.
    return (_lf_shutdown_reactions_size > 0);
}
void terminate_execution() {}
