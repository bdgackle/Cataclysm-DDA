#include "bodypart.h"

/**
 * Temperature state of a single body part
 *
 * Contains all persistant state relevant to the body temperature model for one body part.
 */
struct BodyPartTemperatures {
    /** Current temperature of this body part */
    int current;
    /** Equalibrium temperature of this part, as a function of environment */
    int converging;
    /** Bonus heat that a character has easy control over */
    int bonus;
    /** Counter for frostbite damage */
    int frostbite_counter;
}

class BodyTemperature
{
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
         * Equalize all body parts
         *
         * Heat slowly flows from warmer to colder body parts to simulate blood flow
         */
        void equalize_all_parts();

        /**
         * Do one tick worth of temperature equalization between body parts.
         *
    f    * Calculates heat flow in one direction from a source to a sink body part.  The sink
         * gets warmer if the source is warmer, and vice versa.
         *
         * Rate of flow is controlled by @ref BodyTemperature::equalization_factor
         */
        void equalize_single_part( body_part sink, body_part source );

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

        /** How quickly body part temperatures equalize per tick */
        static const float equalization_factor;
};

/********************************************************************************/

float BodyTemperature::equalization_factor = 0.0001;

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

    // Feet, can put near flame if we are sitting
    const int foot_bonus = fire_intensity * ( is_sitting ? 1000 : 300 );
    bodyparts[pb_foot_l].bonus_warmth += hand_bonus;
    bodyparts[pb_foot_r].bonus_warmth += hand_bonus;
}

void BodyTemperature::apply_bonus_warmth()
{
    // TODO
}

void BodyTemperature::update_temperatures()
{
    // TODO
    for( int i = 0; i < num_pb; i++ ) {
        // Do nothing if this part has already converged
        if( bodyparts[i].current == bodyparts[i].converging ) {
            continue;
        }

        bodyparts[i].current = int( diff *  + bodyparts[i]
    }
}

void BodyTemperature::equalize_single_part( body_part sink, body_part source )
{
    // Body heat is moved around.
    // Shift in one direction only, will be shifted in the other direction separately.
    // If bp1 is warmer, it will lose heat
    int diff = bodyparts[source].current - bodyparts[sink].current;
    int flow = int( diff * equalization_factor );
    bodyparts[sink].current += flow;
}

void BodyTemperature::equalize_all_parts()
{
        // Flow from extremities to torso
        equalize_single_part( bp_torso, bp_arm_l );
        equalize_single_part( bp_torso, bp_arm_r );
        equalize_single_part( bp_torso, bp_leg_l );
        equalize_single_part( bp_torso, bp_leg_r );
        equalize_single_part( bp_torso, bp_head );

        case bp_head:
            // 
            equalize_single_part( bp_head, bp_torso );
            equalize_single_part( bp_head, bp_mouth );
            break;
        case bp_arm_l:
            equalize_single_part( bp_arm_l, bp_torso );
            equalize_single_part( bp_arm_l, bp_hand_l );
            break;
        case bp_arm_r:
            equalize_single_part( bp_arm_r, bp_torso );
            equalize_single_part( bp_arm_r, bp_hand_r );
            break;
        case bp_leg_l:
            equalize_single_part( bp_leg_l, bp_torso );
            equalize_single_part( bp_leg_l, bp_foot_l );
            break;
        case bp_leg_r:
            equalize_single_part( bp_leg_r, bp_torso );
            equalize_single_part( bp_leg_r, bp_foot_r );
            break;
        case bp_mouth:
            equalize_single_part( bp_mouth, bp_head );
            break;
        case bp_hand_l:
            equalize_single_part( bp_hand_l, bp_arm_l );
            break;
        case bp_hand_r:
            equalize_single_part( bp_hand_r, bp_arm_r );
            break;
        case bp_foot_l:
            equalize_single_part( bp_foot_l, bp_leg_l );
            break;
        case bp_foot_r:
            equalize_single_part( bp_foot_r, bp_leg_r );
            break;
        default:
}
