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

#include "../../StdAfx.h"

namespace Arcemu{
	GO_Trap::GO_Trap() : GameObject(){
		spell = NULL;
		targetupdatetimer = 0;
	}

	GO_Trap::GO_Trap( uint64 GUID ) : GameObject( GUID ){
		spell = NULL;
		targetupdatetimer = 0;
	}

	GO_Trap::~GO_Trap(){
	}

	void GO_Trap::InitAI(){
		spell = dbcSpell.LookupEntryForced(  pInfo->trap.spellId );
		charges = pInfo->trap.charges;

		if( pInfo->trap.stealthed != 0 ){
			invisible = true;
			invisibilityFlag = INVIS_FLAG_TRAP;
		}

		cooldown = pInfo->trap.cooldown * 1000;
		if( cooldown < 1000 )
			cooldown = 1000;
		
		maxdistance = sqrt( float( pInfo->trap.radius ) );
		if( maxdistance == 0.0f )
			maxdistance = 1.0f;

		GameObject::InitAI();
	}

	void GO_Trap::Update( unsigned long time_passed ){
		if(m_deleted)
			return;

		if(m_event_Instanceid != m_instanceId){
			event_Relocate();
			return;
		}
		
		if(!IsInWorld())
			return;

		if( spell == NULL )
			return;

		if( GetState() == 1 ){

			targetupdatetimer += time_passed;

			// Update targets only every 2 seconds
			if( targetupdatetimer > cooldown )
				targetupdatetimer = 0;

			if( targetupdatetimer != 0 )
				return;
			
			for( std::set< Object* >::iterator itr = m_objectsInRange.begin(); itr != m_objectsInRange.end(); ++itr ){
				float dist;
				
				Object *o = *itr;

				if( !o->IsUnit() )
					continue;

				if( ( m_summoner != NULL ) && ( o->GetGUID() == m_summoner->GetGUID() ) )
					continue;
				
				dist = GetDistanceSq( o );

				if( dist <=  maxdistance ){

					if(m_summonedGo){
						if(!m_summoner){
							ExpireAndDelete();
							return;
						}
						
						if(!isAttackable(m_summoner, o ))
							continue;
					}
					
					CastSpell( o->GetGUID(), spell );
					
					if( m_summoner != NULL )
						m_summoner->HandleProc( PROC_ON_TRAP_TRIGGER, reinterpret_cast< Unit* >( o ), spell );
					
					if( charges != 0 )
						charges--;

					if(m_summonedGo && pInfo->trap.charges != 0 && charges == 0 ){
						ExpireAndDelete();
						return;
					}
					
					if(spell->EffectImplicitTargetA[0] == 16 ||
						spell->EffectImplicitTargetB[0] == 16){
							return;	 // on area don't continue.
					}
				}
			}
		}
	}
}
