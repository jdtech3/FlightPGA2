#include "utils/audio.hpp"

// -- Globals

static volatile audio::audio_reg_t* audio_reg_ptr_ = reinterpret_cast<audio::audio_reg_t*>(AUDIO_SUBSYSTEM_AUDIO_BASE);
static audio::audio_state_t audio_state_ = {
    .data_array = nullptr,
    .data_array_length = 0,
    .looping = true,
    .volume = 1.f
};
static u32 cur_idx_ = 0;
static bool done_ = false;

// -- ISR

static void audio_isr_(void* context, alt_u32 id) {
    audio::audio_state_t state = *reinterpret_cast<audio::audio_state_t*>(context);
    
    while (audio_reg_ptr_->wslc > 0) {
        audio_reg_ptr_->left_data = static_cast<int>(state.data_array[cur_idx_] * state.volume);
        audio_reg_ptr_->right_data = static_cast<int>(state.data_array[cur_idx_] * state.volume);

        if (cur_idx_ == state.data_array_length) {
            logging::info("Audio ISR", std::string("finished playing, looping: ") + (state.looping ? "yes" : "no"));
            if (!state.looping) {
                audio_reg_ptr_->control = 0;    // disable interrupts
                done_ = true;
                break;
            }
            else cur_idx_ = 0;
        }
        else cur_idx_++;
    }
}

// -- Public

namespace audio {

    void init_isr() {
        // DISABLE interrupts here as we might not want audio to be playing yet
        audio_reg_ptr_->control = 0b1100;   // also clear FIFOs
        audio_reg_ptr_->control = 0b0000;   // stop clearing FIFOs

        // Support both enhanced and legacy APIs
        #ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
            alt_ic_isr_register(
                AUDIO_SUBSYSTEM_AUDIO_IRQ_INTERRUPT_CONTROLLER_ID,
                AUDIO_SUBSYSTEM_AUDIO_IRQ,
                &audio_isr_, &audio_state_, 0
            );
            alt_ic_irq_enable(AUDIO_SUBSYSTEM_AUDIO_IRQ_INTERRUPT_CONTROLLER_ID, AUDIO_SUBSYSTEM_AUDIO_IRQ);
        #else
            alt_irq_register(AUDIO_SUBSYSTEM_AUDIO_IRQ, &audio_state_, &audio_isr_);
            alt_irq_enable(AUDIO_SUBSYSTEM_AUDIO_IRQ);
        #endif
        alt_irq_cpu_enable_interrupts();

        logging::ok("Audio ISR init", "done");
    }

    void play(const int* audio_data, u32 audio_data_length, bool loop) {
        logging::info("Audio", std::string("playing new") + (loop ? " looping" : "") + " sound");
        
        audio_reg_ptr_->control = 0b1100;   // disable interrupt and clear FIFOs

        audio_state_.data_array = audio_data;
        audio_state_.data_array_length = audio_data_length;
        audio_state_.looping = loop;
        audio_state_.volume = 1.f;
        cur_idx_ = 0;
        done_ = false;

        audio_reg_ptr_->control = 0b0010;   // re-enable interrupt
    }

    void stop() {
        logging::info("Audio", "stopping playback");
        audio_reg_ptr_->control = 0b1100;
        audio_reg_ptr_->control = 0;
    }

    void set_volume(float volume) { audio_state_.volume = volume; }
    bool done() { return done_; }

}