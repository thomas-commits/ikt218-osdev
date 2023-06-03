//#include "system.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "hardware.h"
#include "../../apps/include/song.h"


extern uint32_t end; // This is defined in linker.ld

// Define entry point in asm to prevent C++ mangling
extern "C"{
    #include "system.h"
    #include "memory.h"
    #include "isr.h"
    #include "pit.h"

    void kernel_main();
}
enum vga_color {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHT_GREY = 7,
    DARK_GREY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    LIGHT_BROWN = 14,
    WHITE = 15,
};


class OperatingSystem {

public:
    OperatingSystem(vga_color color) {
    }

    void init() {

        printt("Initializing UiA Operating System....\n");
        
    }

    void debug_print(char *str) {
        monitor_write(str);
        
    }

    void interrupt_handler_3(registers_t regs) {
        printt("Called Interrupt Handler 3!\n");
       
    }

    void interrupt_handler_4(registers_t regs) {
        printt("Called Interrupt Handler 4!\n");
       
    }

    void interrupt_handler_5(registers_t regs) {
        printt("Called Interrupt Handler 5!\n");
       
    }

    
};



// Overload the new operator for single object allocation
void* operator new(size_t size) {
    return malloc(size);   // Call the C standard library function malloc() to allocate memory of the given size and return a pointer to it
}

// Overload the delete operator for single object deallocation
void operator delete(void* ptr) noexcept {
    free(ptr);             // Call the C standard library function free() to deallocate the memory pointed to by the given pointer
}

// Overload the new operator for array allocation
void* operator new[](size_t size) {
    return malloc(size);   // Call the C standard library function malloc() to allocate memory of the given size and return a pointer to it
}

// Overload the delete operator for array deallocation
void operator delete[](void* ptr) noexcept {
    free(ptr);             // Call the C standard library function free() to deallocate the memory pointed to by the given pointer
}



// Main entry point for kernel
void kernel_main()
{

    init_kernel_memory(&end);

    monitor_clear();

    auto os = OperatingSystem(WHITE);
    os.init();

    

    // Initializes the descriptor tables, and prints to indicate
    // that the GDT has been successfully initialized
    printt("Initializing GDT....\n");
    init_descriptor_tables();
    printt("GDT initialized!\n");
    


    register_interrupt_handler(3,[](registers_t* regs, void* context){
        auto* os = (OperatingSystem*)context;
        os->interrupt_handler_3(*regs);
    }, (void*)&os);

    // Create some interrupt handler for 4
    register_interrupt_handler(4,[](registers_t* regs, void* context){
        auto* os = (OperatingSystem*)context;
        os->interrupt_handler_4(*regs);
    }, (void*)&os);

    // Create some interrupt handler for 5
    register_interrupt_handler(5,[](registers_t* regs, void* context){
        auto* os = (OperatingSystem*)context;
        os->interrupt_handler_5(*regs);
    }, (void*)&os);


    // Fire interrupts! Should trigger callback above
    /*asm volatile ("int $0x3");
    asm volatile ("int $0x4");
    asm volatile ("int $0x5");*/


    asm volatile("sti");
   
   
    // Printing keyboard input to screen
    Keyboard::hook_keyboard([](uint8_t scancode, void* context){
        auto* os = (OperatingSystem*)context;
        monitor_put(Keyboard::scancode_to_ascii(scancode));
    }, &os);

    // Initialize Paging
    init_paging(); // <------ THIS IS PART OF THE ASSIGNMENT

     // Print memory layout
    print_memory_layout(); // <------ THIS IS PART OF THE ASSIGNMENT

    // Allocate some memory using kernel memory manager
	// THIS IS PART OF THE ASSIGNMENT
    void* some_memory = malloc(12345); 
    void* memory2 = malloc(54321); 
    void* memory3 = malloc(13331);
    char* memory4 = new char[1000]();



     init_pit();  // Initialize the PIT

    

    // Start the main execution loop

    /*int counter = 0;
    while (true) {
        printt("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
        sleep_busy(1000);
        printt("[%d]: Slept using busy-waiting.\n", counter++);

        printt("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
        sleep_interrupt(1000);
        printt("[%d]: Slept using interrupts.\n", counter++);
    }*/

    sleep_busy(3000);
    

    // Music player
    Song* songs[] = {
        new Song(music_1, sizeof(music_1) / sizeof(Note)),
        //new Song(music_6, sizeof(music_6) / sizeof(Note)),
        //new Song(music_5, sizeof(music_5) / sizeof(Note)),
        //new Song(music_4, sizeof(music_4) / sizeof(Note)),
        //new Song(music_3, sizeof(music_3) / sizeof(Note)),
        //new Song(music_2, sizeof(music_2) / sizeof(Note))
    };
    uint32_t n_songs = sizeof(songs) / sizeof(Song*);

    SongPlayer* player = create_song_player();


    
	    for(uint32_t i =0; i < n_songs; i++){
	        printt("Playing Song...\n");
	        player->play_song(songs[i]);
	        printt("Finished playing the song.\n");
	    };

     sleep_busy(1500);
     monitor_clear();

    


    while (1)
    {}
    
}



