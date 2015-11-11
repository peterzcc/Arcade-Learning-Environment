/* *****************************************************************************
 * The lines 57, 110, 119 and 127 are based on Xitari's code, from Google Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 * *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2013 by Yavar Naddaf, Joel Veness, Marc G. Bellemare and
 *   the Reinforcement Learning and Artificial Intelligence Laboratory
 * Released under the GNU General Public License; see License.txt for details.
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * *****************************************************************************
 */
#include "Superbreakout.hpp"

#include "../RomUtils.hpp"


SuperbreakoutSettings::SuperbreakoutSettings() {

    reset();
}


/* create a new instance of the rom */
RomSettings* SuperbreakoutSettings::clone() const {

    RomSettings* rval = new SuperbreakoutSettings();
    *rval = *this;
    return rval;
}
/*
super breakout score 0xDC 0xDD

life 0xE1
0xF9 = FF : not dead, F7: dead
0xE0 >=0: has started
*/
/* process the latest information from ALE */
void SuperbreakoutSettings::step(const System& system) {

    // update the reward
    reward_t score = getDecimalScore(0xDD, 0xDC, &system);
    m_reward = score - m_score;
    m_score = score;
    // update terminal status
    m_terminal = readRam(&system, 0xF9) != 0xFF;

    m_lives = 6 - readRam(&system, 0xE1) - (m_terminal);
}


/* is end of game */
bool SuperbreakoutSettings::isTerminal() const {

    return m_terminal;
};


/* get the most recently observed reward */
reward_t SuperbreakoutSettings::getReward() const {

    return m_reward;
}


/* is an action part of the minimal set? */
bool SuperbreakoutSettings::isMinimal(const Action &a) const {

    switch (a) {
        case PLAYER_A_NOOP:
        case PLAYER_A_FIRE:
        case PLAYER_A_RIGHT:
        case PLAYER_A_LEFT:
            return true;
        default:
            return false;
    }
}


/* reset the state of the game */
void SuperbreakoutSettings::reset() {

    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;
    m_lives    = 5;
}


/* saves the state of the rom settings */
void SuperbreakoutSettings::saveState(Serializer & ser) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
  ser.putInt(m_lives);
}

// loads the state of the rom settings
void SuperbreakoutSettings::loadState(Deserializer & ser) {
  m_reward = ser.getInt();
  m_score = ser.getInt();
  m_terminal = ser.getBool();
  m_lives = ser.getInt();
}
