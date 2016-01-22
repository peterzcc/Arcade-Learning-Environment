/* *****************************************************************************
 * The lines 54 - 59, 100, 109 and 117 are based on Xitari's code, from Google Inc.
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
#include "Galaxnew.hpp"

#include "../RomUtils.hpp"

ActionVect GalaxnewSettings::actions;

GalaxnewSettings::GalaxnewSettings() {
    reset();
}


/* create a new instance of the rom */
RomSettings* GalaxnewSettings::clone() const {

    RomSettings* rval = new GalaxnewSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void GalaxnewSettings::step(const System& system) {

  // update the reward
  int score = getDecimalScore(0xAE, 0xAD,0xAC, &system);
  // reward cannot get negative in this game. When it does, it means that the score has looped
  // (overflow)
  m_reward = score - m_score;

  m_score = score;
  m_lives = readRam(&system,0xB9);
  // update terminal status
  // If bit 0x80 is on, then game is over
  m_terminal = (m_lives <= 0);
}


/* is end of game */
bool GalaxnewSettings::isTerminal() const {

    return m_terminal;
};


/* get the most recently observed reward */
reward_t GalaxnewSettings::getReward() const {

    return m_reward;
}


/* is an action part of the minimal set? */
bool GalaxnewSettings::isMinimal(const Action &a) const {

    switch (a) {
        case PLAYER_A_NOOP:
        case PLAYER_A_LEFT:
        case PLAYER_A_RIGHT:
        case PLAYER_A_FIRE:
        case PLAYER_A_LEFTFIRE:
        case PLAYER_A_RIGHTFIRE:
            return true;
        default:
            return false;
    }
}


/* reset the state of the game */
void GalaxnewSettings::reset() {

    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;
    m_lives    = 2;
}


/* saves the state of the rom settings */
void GalaxnewSettings::saveState(Serializer & ser) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
  ser.putInt(m_lives);
}

// loads the state of the rom settings
void GalaxnewSettings::loadState(Deserializer & ser) {
  m_reward = ser.getInt();
  m_score = ser.getInt();
  m_terminal = ser.getBool();
  m_lives = ser.getInt();
}
