#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]
/*
* MIT License
*
* Copyright (c) 2023 bitten2up
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/


/////////////////////////////////////////////////////////
//  saving functionality
/////////////////////////////////////////////////////////
//  done in rust to keep shit safe
/////////////////////////////////////////////////////////

#![allow(dead_code)]
mod bittendef;


// i will define this here instead of using bindgen
// so then i have control over it
use std::path::Path;
extern crate savefile;
use savefile::prelude::*;

#[macro_use]
extern crate savefile_derive;

#[derive(Savefile)]
pub struct bitSave {
    // first 7 bytes is the header, used to make sure that we are using the right type of file
    header:     [i8;8],
    // the next byte is the version
    version:    i8,
    // the next 2 bytes is our x position
    xpos:       i16,
    // the next 2 bytes is our y position
    ypos:       i16,
    // the next bit is to see if music is enabled
    music:      bool,
}
fn save_data(save_data: &bitSave) {
    save_file("bitten.sav", 0, save_data).unwrap();
}
// sadly didn't work
// const  default_save = bitSave { header: [0x42, 0x49, 0x54, 0x53, 0x41, 0x56, 0x00, 0x7f], version: 0x01, xpos: 0, ypos: 0, music: true };
fn reset_save() {
    println!("reset save");
    let tmp_save = bitSave { header: [0x42, 0x49, 0x54, 0x53, 0x41, 0x56, 0x00, 0x7f], version: 0x01, xpos: 0, ypos: 0, music: true };
    save_data(&tmp_save);
    //free(tmp_save);
}

#[no_mangle]
pub extern "C" fn saveGame(game_state: *mut bittendef::bit_game){
    println!("savegame");
    // create a temperay save to pass to save_data
    unsafe
    {
        let tmp_save = bitSave { header: [0x42, 0x49, 0x54, 0x53, 0x41, 0x56, 0x00, 0x7f], version: 0x01, xpos: (*game_state).player.x, ypos: (*game_state).player.y, music: (*game_state).settings.audio }; // get our savedata
        save_data(&tmp_save);
        //free(tmp_save);
    }
}
fn load_data() -> bitSave {
    load_file("bitten.sav", 0).unwrap()
}
#[no_mangle]
pub extern "C" fn loadGame(game_state: *mut bittendef::bit_game) {
    println!("load game");
    if !Path::new("./bitten.sav").exists() {
        reset_save();
        let mut save_data = bitSave { header: [0x42, 0x49, 0x54, 0x53, 0x41, 0x56, 0x00, 0x7f], version: 0x01, xpos: 0, ypos: 0, music: true };
        unsafe {
            (*game_state).player.x = save_data.xpos;
            (*game_state).player.y = save_data.ypos;
            (*game_state).settings.audio = save_data.music;
        }
        return;
    }
    let mut save_data = load_data();
    if save_data.header != [0x42, 0x49, 0x54, 0x53, 0x41, 0x56, 0x00, 0x7f] {
        // our saved data is corrupt
        reset_save();
        save_data = bitSave { header: [0x42, 0x49, 0x54, 0x53, 0x41, 0x56, 0x00, 0x7f], version: 0x01, xpos: 0, ypos: 0, music: true };
    }
    else if save_data.version > 0x01 {
        unsafe {(*game_state).invalidSave=true;}
    }
    unsafe {
        (*game_state).player.x = save_data.xpos;
        (*game_state).player.y = save_data.ypos;
        (*game_state).settings.audio = save_data.music;
        println!("{}", (*game_state).settings.audio);
    }
    //free(save_data)
}