#include "bodypart.h"

/**
 * Temperature state of a single body part
 *
 * Contains all persistant state relevant to the body temperature model for one body part.
 */
struct BodyPartTemperatures {
    /** Current temperature of this body part */
    int current;
    /** Temperature that this part is currently converging towards */
    int converging;
    /** Bonus heat that a character has easy control over */
    int bonus;
    /** Counter for frostbite damage */
    int frostbite_counter;
}

class BodyTemperature {
        /**
         * Add heat to body parts because of huddling over fire
         *
         * If the character is awake and actively attempting to warm up near a fire, then
         * additional bonuses are applied to each body part.
         *
         * The strength of this effect is proportional to the size of the fire, and is
         * stronger for hands and arms that can be held over the fire easily than for more
         * central body parts.
         *
         * The feet are a special case.  A character sitting on a chair or similar is able
         * to hold his or her feet over the fire, and gets a bonus.  A character standing
         * by the fire gets little additional foot warming.
         *
         * @param fire_intensity Size of fire being used to warm up
         * @param is_sitting If true, character can lift feet over fire
         */
        int add_bonus_fire_warmth( int fire_intensity, bool is_sitting );

        /**
         * Apply all types of "bonus" heat sources to all body parts.
         *
         * Certain types of warmth are "optional" for a character and are only applied if the
         * body part is below the optimum temperature for comfort.  As an example, you wouldn't
         * continue to hold your hands over the fire if the fire was too hot, so we don't apply
         * that warmth source beyond the level of overheating.
         */
        void apply_bonus_warmth();

        /**
         * Update the model for one tick of heat flow.
         *
         * Causes the actual temperatures to converge toward their current end targets by one
         * iteration.
         */
        void update_temperatures();

    private:
        /** Temperature state data for all body parts */
        std::array<BodyPartModel, num_pb> bodyparts;
};

/********************************************************************************/

void BodyTemperature::add_bonus_fire_warmth( int fire_intensity, bool is_sitting )
{
    // Body core, can't easily extend over fire
    const int core_bonus = fire_intensity * fire_intensity * 150;
    bodyparts[bp_head].bonus_warmth += core_bonus;
    bodyparts[bp_torso].bonus_warmth += core_bonus;
    bodyparts[bp_mouth].bonus_warmth += core_bonus;
    bodyparts[bp_leg_l].bonus_warmth += core_bonus;
    bodyparts[bp_leg_r].bonus_warmth += core_bonus;

    // Arms, can partially extend over fire
    const int arm_bonus = fire_intensity * 600;
    bodyparts[pb_arm_l].bonus_warmth += arm_bonus;
    bodyparts[pb_arm_r].bonus_warmth += arm_bonus;

    // Hands, can easily place right next to flame
    const int hand_bonus = fire_intensity * 1500;
    bodyparts[pb_hand_l].bonus_warmth += hand_bonus;
    bodyparts[pb_hand_r].bonus_warmth += hand_bonus;

    // Feet, can put near flame if we can physically get them there
    const int foot_bonus = fire_intensity * (is_sitting ? 1000 : 300);
    bodyparts[pb_foot_l].bonus_warmth += hand_bonus;
    bodyparts[pb_foot_r].bonus_warmth += hand_bonus;
}

void BodyTemperature::apply_bonus_warmth()
{
}

void BodyTemperature::update_temperatures()
{
    for( int i = 0; i < num_pb; i++ ) {
        // Do nothing if this part has already converged
        if( bodyparts[i].current == bodyparts[i].converging ) {
            continue;
        }

        bodyparts[i].current = int( diff *  + bodyparts[i]
    }
}
