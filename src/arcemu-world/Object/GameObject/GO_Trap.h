/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2010 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef GO_TRAP_HPP
#define GO_TRAP_HPP

namespace Arcemu{
	/////////////////////////////////////////////////////////////////////
	//class GO_Trap
	//  Class implementing Type 6 (TRAP) GameObjects
	//
	////////////////////////////////////////////////////////////////////
	class GO_Trap : public GameObject{

	public:
		GO_Trap();
		
		
		GO_Trap( uint64 GUID );
		
		
		~GO_Trap();
		
		
		void InitAI();
		
		
		void Update( unsigned long time_passed );
	
	private:
		SpellEntry *spell;
		uint32 targetupdatetimer;
		float maxdistance;
		uint32 cooldown;
	};

}

#endif
