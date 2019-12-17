/* MenuClass.cpp */
#include "stdafx.h"

#include "menu.h"

#include <locale>
#include <utility>

#include "menucontrols.h"
#include "menuutils.h"
#include "menumemutils.hpp"

// TODO: Fixes:
//      - Reduce code duplication (titles, OptionPlus title)
//      - Handle Chalet London scaling

// TODO: Improvements:
//      - Mouse support
//      - Badges?

// TODO: Never:
//      - Re-write to OO

namespace NativeMenu {
void Menu::Initialize() {
    mRecordGlobal = FindRecordGlobal();
}

void Menu::SetFiles(const std::string &fileName) {
    settings.SetFiles(fileName);
}

void Menu::ReadSettings() {
    printf("\n readSettings");

    settings.ReadSettings(&controls, this);
    menuX = menuX + menuWidth / 2.0f;
}

void Menu::RegisterOnMain(std::function<void()> onMain) {
    this->onMain = std::move(onMain);
}

void Menu::RegisterOnExit(std::function<void()> onExit) {
    this->onExit = std::move(onExit);
}

bool Menu::CurrentMenu(const std::string& menuname) {
    return menuname == actualmenu;
}

/*
 * Section Headers/Footers
 */
void Menu::fitTitle(std::string &title, float &newSize, float titleSize) {

}

void Menu::Title(const std::string& title) {
    std::string newTitle = title;
    Title(newTitle, textureDicts[titleTextureIndex], textureNames[titleTextureIndex], 0.65f);
}

void Menu::Title(const std::string& title, float customSize) {
    Title(title, textureDicts[titleTextureIndex], textureNames[titleTextureIndex], customSize);
}

void Menu::Title(const std::string& title, const std::string& dict, const std::string& texture) {
    std::string newTitle = title;
    Title(newTitle, dict, texture, 0.65f);
}

void Menu::Title(const std::string& title, const std::string& dict, const std::string& texture, float customSize) {
    optioncount = 0;
    totalHeight = 0.0f;

    float titletexty = menuY + totalHeight + titleTextOffset + titleTextOffset * 2.0f * (titleTextSize - customSize);
    float titley = menuY + totalHeight + titleTextureOffset;
    unsigned newlines = 0;
    for (const auto& c : title) {
        if (c == '\n')
            newlines++;
    }
    for (unsigned i = 0; i < newlines; ++i) {
        titletexty -= titleHeight / 5.0f;
    }

    textDraws.push_back([=]() { drawText(title, titleFont, menuX, titletexty, customSize, customSize, titleTextColor, 0); });
    backgroundSpriteDraws.push_back(
        [=]() { drawSprite(dict, texture,
            menuX, titley, menuWidth, titleHeight, 0.0f, titleBackgroundColor); });

    totalHeight = titleHeight;
    headerHeight = titleHeight;
}

void Menu::Title(const std::string& title, int textureHandle) {
    float newSize;
    std::string newTitle = title;
    fitTitle(newTitle, newSize, titleTextSize);
    Title(newTitle, textureHandle, newSize);
}

void Menu::Title(const std::string& title, int textureHandle, float customSize) {
    optioncount = 0;
    totalHeight = 0.0f;

    float titletexty = menuY + totalHeight + titleTextOffset + titleTextOffset * 2.0f * (titleTextSize - customSize);
    float titley = menuY + totalHeight + titleTextureOffset;
    unsigned newlines = 0;
    for (const auto& c : title) {
        if (c == '\n')
            newlines++;
    }
    for (unsigned i = 0; i < newlines; ++i) {
        titletexty -= titleHeight / 5.0f;
    }

    textDraws.push_back(
        [=]() { drawText(title, titleFont, menuX, titletexty, customSize, customSize, titleTextColor, 0); });


    float titleX = menuX;



    float drawWidth = menuWidth;
    // handle multi-monitor setups
   
    totalHeight = titleHeight;
    headerHeight = titleHeight;
}

void Menu::Subtitle(const std::string& subtitle) {    
    float subtitleY = subtitleTextureOffset + menuY + totalHeight;
    float subtitleTextY = menuY + totalHeight;
    textDraws.push_back(
        [=]() { drawText(subtitle, optionsFont, (menuX - menuWidth / 2.0f) + menuTextMargin, 
                  subtitleTextY, subtitleTextSize, subtitleTextSize, titleTextColor, 1); }
    );

    backgroundRectDraws.push_back(
        [=]() { drawRect(
        menuX, subtitleY, menuWidth, subtitleHeight, solidBlack); }
    );

    totalHeight += subtitleHeight;
    headerHeight += subtitleHeight;
}

void Menu::Footer(Color color) {
    footerType = FooterType::Color;
    footerColor = color;
}

void Menu::Footer(const std::string& dict, const std::string& texture) {
    footerType = FooterType::Sprite;
    footerSprite.Dictionary = dict;
    footerSprite.Name = texture;
}

/*
 * Section Options
 */
bool Menu::Option(const std::string& option, const std::vector<std::string>& details) {
    return Option(option, optionsBackgroundSelectColor, details);
}

bool Menu::Option(const std::string& option, Color highlight, const std::vector<std::string>& details) {
    optioncount++;

    bool highlighted = currentoption == optioncount;


    bool visible = false;
    float optiony;
    float optiontexty;

    if (currentoption <= maxDisplay && optioncount <= maxDisplay) {
        visible = true;
        optiontexty = menuY + totalHeight;
        optiony = optiontexty + optionTextureOffset;
    }
    else if (optioncount > currentoption - maxDisplay && optioncount <= currentoption) {
        visible = true;
        optiontexty = menuY + headerHeight + 
            (optioncount - (currentoption - maxDisplay + 1)) * optionHeight;
        optiony = optiontexty + optionTextureOffset;
    }

    if (visible) {
        const float chaletLondonMult = optionsFont == 0 ? 0.75f : 1.0f;
        bool appendDots = false;
        std::string optionTrunc = option;
  
        if (appendDots) {
            optionTrunc.pop_back();
            optionTrunc.pop_back();
            optionTrunc.pop_back();
            optionTrunc += "...";
        }
        textDraws.push_back([=]() {
            drawText(optionTrunc, optionsFont,
                (menuX - menuWidth / 2.0f) + menuTextMargin, 
                optiontexty, 
                optionTextSize, optionTextSize, 
                highlighted ? optionsTextSelectColor : optionsTextColor, 1);
        });

        if (highlighted) {
            float highlightY = optiony;
            if (useSmoothScroll) {
                highlightY = lerp(oldSmoothY,
                    optiony,
                    1.0f);
                oldSmoothY = highlightY;
            }
            highlightsSpriteDraws.push_back(
                [=]() { drawSprite(textureDicts[highlTextureIndex], textureNames[highlTextureIndex],
                    menuX, highlightY, menuWidth, optionHeight, 0.0f, highlight); }
            );

            if (!details.empty()) {
                this->details = details;
            }
        }
    }

    totalHeight += optionHeight;
    return optionpress && currentoption == optioncount;
}

bool Menu::MenuOption(const std::string& option, const std::string& menu, const std::vector<std::string>& details) {
    Option(option, details);
    float indicatorHeight = totalHeight - optionHeight; // why the hell was this menu designed like *this*?
    bool highlighted = currentoption == optioncount;

    if (currentoption <= maxDisplay && optioncount <= maxDisplay) {
        textDraws.push_back(
            [=]() { drawText(
            "2", 3,
            menuX + menuWidth / 2.0f - optionRightMargin,
            indicatorHeight + menuY,
            optionTextSize * 0.75f, optionTextSize * 0.75f,
            highlighted ? optionsTextSelectColor : optionsTextColor, 2
            ); });
    }
    else if ((optioncount > (currentoption - maxDisplay)) && optioncount <= currentoption) {
        int optioncount_ = optioncount;
        textDraws.push_back(
            [=]() { drawText(
            "2", 3,
            menuX + menuWidth / 2.0f - optionRightMargin,
            menuY + headerHeight + (optioncount_ - (currentoption - maxDisplay + 1)) * optionHeight,
            optionTextSize * 0.75f, optionTextSize * 0.75f,
            highlighted ? optionsTextSelectColor : optionsTextColor, 2
        ); });
    }

    if (optionpress && currentoption == optioncount) {
        optionpress = false;
        changeMenu(menu);
        return true;
    }
    return false;
}

bool Menu::OptionPlus(const std::string& option, const std::vector<std::string>& extra, bool *_highlighted,
                      const std::function<void() >& onRight, const std::function<void() >& onLeft,
                      const std::string& title, const std::vector<std::string>& details) {
    Option(option, details);
    size_t infoLines = extra.size();
    bool highlighted = currentoption == optioncount;
    if (_highlighted != nullptr) {
        *_highlighted = highlighted;
    }

    if (currentoption == optioncount) {
        if (onLeft && leftpress) {
            onLeft();
            leftpress = false;
            return false;
        }
        if (onRight && rightpress) {
            onRight();
            rightpress = false;
            return false;
        }
    }

    if (highlighted && infoLines > 0 && 
        ((currentoption <= maxDisplay && optioncount <= maxDisplay) ||
        ((optioncount > (currentoption - maxDisplay)) && optioncount <= currentoption))) {
        drawOptionPlusExtras(extra, title);
    }

    return optionpress && currentoption == optioncount;
}

void Menu::OptionPlusPlus(const std::vector<std::string>& extra, const std::string& title) {
    drawOptionPlusExtras(extra, title);
}

bool Menu::IntOption(const std::string& option, int &var, int min, int max, int step, const std::vector<std::string>& details) {
    Option(option, details);
    std::string printVar = std::to_string(var);
    bool highlighted = currentoption == optioncount;

    drawOptionValue(printVar, highlighted, max - min);
    return processOptionItemControls(var, min, max, step);
}

bool Menu::FloatOption(const std::string& option, float &var, float min, float max, float step,
                       const std::vector<std::string>& details) {
    Option(option, details);
    unsigned precision = behindDec(step);
    if (precision < 2) precision = 2;
    if (precision > 6) precision = 6;

    char buf[100];
    _snprintf_s(buf, sizeof(buf), "%.*f", precision, var);
    std::string printVar = buf;
    int items = min != max ? 1 : 0;

    bool highlighted = currentoption == optioncount;
    
    drawOptionValue(printVar, highlighted, items);
    return processOptionItemControls(var, min, max, step);
}

bool Menu::BoolOption(const std::string& option, bool &var, const std::vector<std::string>& details) {
    Option(option, details);
    float indicatorHeight = totalHeight - optionHeight;
    bool highlighted = currentoption == optioncount;
    
    char *tickBoxTexture;
    Color optionColors = optionsTextColor;
    float boxSz = 0.0475f;

    if (highlighted) {
        tickBoxTexture = var ? const_cast<char*>("shop_box_tickb") : const_cast<char*>("shop_box_blankb");
    }
    else {
        tickBoxTexture = var ? const_cast<char*>("shop_box_tick") : const_cast<char*>("shop_box_blank");
    }

    bool doDraw = false;
    float textureY;
    
    if (currentoption <= maxDisplay && optioncount <= maxDisplay) {
        doDraw = true;
        textureY = (indicatorHeight + (menuY + 0.0175f));
    }
    else if ((optioncount > (currentoption - maxDisplay)) && optioncount <= currentoption) {
        doDraw = true;
        textureY = menuY + headerHeight + (optioncount - (currentoption - maxDisplay + 1)) * optionHeight + 0.0175f;
    }

    if (doDraw) {
        int resX, resY;
        GRAPHICS::GET_SCREEN_RESOLUTION(&resX, &resY);
        float ratio = static_cast<float>(resX) / static_cast<float>(resY);
        foregroundSpriteCalls.push_back(
            [=]() { drawSprite("commonmenu", tickBoxTexture,
                menuX + menuWidth / 2.0f - optionRightMargin, textureY, boxSz / ratio, boxSz, 0.0f, optionColors)
        ; });
    }

    if (optionpress && currentoption == optioncount) {
        var ^= 1;
        return true;
    }
    return false;
}

bool Menu::BoolSpriteOption(const std::string& option, bool enabled, std::string category, const std::string& spriteOn,
                            const std::string& spriteOff, const std::vector<std::string>& details) {
    Option(option, details);
    float indicatorHeight = totalHeight - optionHeight;
    bool highlighted = currentoption == optioncount;
    
    if (currentoption <= maxDisplay && optioncount <= maxDisplay) {
        foregroundSpriteCalls.push_back(
            [=]() { drawSprite(category, enabled ? spriteOn : spriteOff,
            menuX + menuWidth/2.0f - optionRightMargin, 
            (indicatorHeight + (menuY + 0.016f)), 
            0.03f, 0.05f, 0.0f, highlighted ? optionsTextSelectColor : optionsTextColor); });
    }
    else if ((optioncount > (currentoption - maxDisplay)) && optioncount <= currentoption) {
        int optioncount_ = optioncount;
        foregroundSpriteCalls.push_back(
            [=]() { drawSprite(category, enabled ? spriteOn : spriteOff,
            menuX + menuWidth/2.0f - optionRightMargin, 
            menuY + headerHeight + (optioncount_ - (currentoption - maxDisplay + 1)) * optionHeight + 0.016f,
            0.03f, 0.05f, 0.0f, highlighted ? optionsTextSelectColor : optionsTextColor); });
    }

    return optionpress && currentoption == optioncount;
}

bool Menu::IntArray(const std::string& option, std::vector<int> display, int &iterator, const std::vector<std::string>& details) {
    Option(option, details);
    bool highlighted = currentoption == optioncount;

    std::string printVar = std::to_string(display[iterator]);
    
    int min = 0;
    int max = static_cast<int>(display.size()) - 1;
    
    drawOptionValue(printVar, highlighted, max);
    return processOptionItemControls(iterator, min, max, 1);
}

bool Menu::FloatArray(const std::string& option, std::vector<float> display, int &iterator,
                      const std::vector<std::string>& details) {
    Option(option, details);
    bool highlighted = currentoption == optioncount;
    int min = 0;
    int max = static_cast<int>(display.size()) - 1;

    if (iterator > static_cast<int>(display.size()) || iterator < 0) {
        drawOptionValue("error", highlighted, max);
        return false;
    }

    unsigned precision = behindDec(display[iterator]);
    if (precision < 2) precision = 2;
    if (precision > 6) precision = 6;

    char buf[100];
    _snprintf_s(buf, sizeof(buf), "%.*f", precision, display[iterator]);
    std::string printVar = buf;

    drawOptionValue(printVar, highlighted, max);
    return processOptionItemControls(iterator, min, max, 1);
}

bool Menu::StringArray(const std::string& option, const std::vector<std::string>& display, int &iterator,
                       const std::vector<std::string>& details) {
    Option(option, details);
    bool highlighted = currentoption == optioncount; 
    int min = 0;
    int max = static_cast<int>(display.size()) - 1;

    if (iterator > static_cast<int>(display.size()) || iterator < 0) {
        drawOptionValue("error (" + std::to_string(iterator) + ")", highlighted, max);
        return false;
    }

    std::string printVar = display[iterator];
    drawOptionValue(printVar, highlighted, max);
    return processOptionItemControls(iterator, min, max, 1);
}


/*
 * Section Draw
 */
void Menu::drawInstructionalButtons() {
    //std::vector<InstructionalButton> instructionalButtons;
   // Scaleform instructionalButtonsScaleform("instructional_buttons");

   // instructionalButtonsScaleform.CallFunction("CLEAR_ALL");
   // instructionalButtonsScaleform.CallFunction("TOGGLE_MOUSE_BUTTONS", { 0 });
   // instructionalButtonsScaleform.CallFunction("CREATE_CONTAINER");
   // instructionalButtonsScaleform.CallFunction("SET_DATA_SLOT", { 0, std::string(PAD::GET_CONTROL_INSTRUCTIONAL_BUTTON(2, ControlPhoneSelect, 0)), std::string(UI::_GET_LABEL_TEXT("HUD_INPUT2")) });
   // instructionalButtonsScaleform.CallFunction("SET_DATA_SLOT", { 1, std::string(PAD::GET_CONTROL_INSTRUCTIONAL_BUTTON(2, ControlPhoneCancel, 0)), std::string(UI::_GET_LABEL_TEXT("HUD_INPUT3")) });
   // instructionalButtonsScaleform.CallFunction("SET_DATA_SLOT", { 2, std::string(PAD::GET_CONTROL_INSTRUCTIONAL_BUTTON(2, ControlPhoneUp, 0)), std::string("Next option") });
   // instructionalButtonsScaleform.CallFunction("SET_DATA_SLOT", { 3, std::string(PAD::GET_CONTROL_INSTRUCTIONAL_BUTTON(2, ControlPhoneDown, 0)), std::string("Previous option") });

    //int count = 2;
    //for (const auto& button : instructionalButtons) {
    //    if (button.ItemBind == null || MenuItems[CurrentSelection] == button.ItemBind) {
    //        instructionalButtonsScaleform.CallFunction("SET_DATA_SLOT", { count, button.GetButtonId(), button.Text });
    //        count++;
    //    }
    //}

   // instructionalButtonsScaleform.CallFunction("DRAW_INSTRUCTIONAL_BUTTONS", { -1 });
   // instructionalButtonsScaleform.Render2D();
}

void Menu::drawMenuDetails() {
    int maxOptionCount = optioncount >  maxDisplay ?  maxDisplay : optioncount;

    float footerTextY;
    float footerBackY;

    if (optioncount > maxDisplay) {
        footerTextY = maxDisplay * optionHeight + menuY + headerHeight;
    }
    else {
        footerTextY = totalHeight + menuY;
    }
    footerBackY = footerTextY + optionTextureOffset;

    switch (footerType) {
        case FooterType::Color: {
            backgroundRectDraws.push_back(
                [=]() { drawRect(
                          menuX, footerBackY, menuWidth, optionHeight, footerColor); }
            );
            break;
        }
        case FooterType::Sprite: {
            backgroundSpriteDraws.push_back(
                [=]() { drawSprite(footerSprite.Dictionary, footerSprite.Name,
                          menuX, footerBackY, menuWidth, optionHeight, 0.0f, titleBackgroundColor); }
            );
            break;
        }
        default: {
            backgroundRectDraws.push_back(
                [=]() { drawRect(
                          menuX, footerBackY, menuWidth, optionHeight, solidBlack); }
            );
            break;
        }
    }
    
    textDraws.push_back(
        [=]() { drawText(std::to_string(currentoption) + " / " + std::to_string(optioncount),
                  optionsFont, menuX - 0.1f, footerTextY, optionTextSize, optionTextSize, titleTextColor, 2); }
    );

    // Options background
    backgroundSpriteDraws.push_back(
        [=]() { drawSprite(textureDicts[backgTextureIndex], textureNames[backgTextureIndex], 
                  menuX, 
                  menuY + headerHeight + maxOptionCount * optionHeight / 2,
                  menuWidth, 
                  optionHeight * maxOptionCount, 0.0f, optionsBackgroundColor); }
    );

    // Menu detail box
    if (!details.empty()) {
        drawMenuDetails(details, footerBackY + optionHeight / 1.5f);
    }

    // Indicators
    if (currentoption == 1) {
        foregroundSpriteCalls.push_back(
            [=]() { drawSprite("commonmenu", "arrowright",
                      menuX, 
                      (footerTextY + 0.0175f),
                      0.02f, 0.02f, 90.0f, titleTextColor); }
        );
    }
    else if (currentoption == optioncount) {
        foregroundSpriteCalls.push_back(
            [=]() { drawSprite("commonmenu", "arrowright",
                      menuX, 
                      (footerTextY + 0.0175f),
                      0.02f, 0.02f, 270.0f, titleTextColor); }
        );
    }
    else {
        foregroundSpriteCalls.push_back(
            [=]() { drawSprite("commonmenu", "arrowright",
                      menuX, 
                      (footerTextY + 0.0125f),
                      0.02f, 0.02f, 270.0f, titleTextColor); }
        );
        foregroundSpriteCalls.push_back(
            [=]() { drawSprite("commonmenu", "arrowright",
                      menuX, 
                      (footerTextY + 0.0225f),
                      0.02f, 0.02f, 90.0f, titleTextColor); }
        );
    }
}

void Menu::EndMenu() {
    if (menulevel < 1)
        return;

    drawMenuDetails();

    // GRAPHICS::_SCREEN_DRAW_POSITION_BEGIN(76, 84);
    // GRAPHICS::_SCREEN_DRAW_POSITION_RATIO(0, 0, 0, 0);
    for (const auto& f : backgroundSpriteDraws) { f(); }
    for (const auto& f : backgroundRectDraws)   { f(); }
    for (const auto& f : highlightsSpriteDraws) { f(); }
    for (const auto& f : foregroundSpriteCalls) { f(); }
    for (const auto& f : textDraws)             { f(); }
    // GRAPHICS::_SCREEN_DRAW_POSITION_END();

    backgroundSpriteDraws.clear();
    backgroundRectDraws.clear();
    highlightsSpriteDraws.clear();
    foregroundSpriteCalls.clear();
    textDraws.clear();
    details.clear();
    footerType = FooterType::Default;

    hideHUDComponents();
    disableKeys();
    drawInstructionalButtons();
}

/*
 * Section Inputs
 */
void Menu::CheckKeys() {
    controls.Update();
    optionpress = false;

   if (GetTickCount64() - delay > menuTime ||
      controls.IsKeyJustPressed(MenuControls::MenuKey) ||
      controls.IsKeyJustPressed(MenuControls::MenuSelect) ||
      controls.IsKeyJustPressed(MenuControls::MenuCancel) ||
      controls.IsKeyJustPressed(MenuControls::MenuUp) ||
      controls.IsKeyJustPressed(MenuControls::MenuDown) ||
      controls.IsKeyJustPressed(MenuControls::MenuLeft) ||
      controls.IsKeyJustPressed(MenuControls::MenuRight)) {
       processMenuNav();
    }

    if (controls.IsKeyJustReleased(MenuControls::MenuKey) || controls.IsKeyJustPressed(MenuControls::MenuKey) ||
        controls.IsKeyJustReleased(MenuControls::MenuSelect) || controls.IsKeyJustPressed(MenuControls::MenuSelect) ||
        controls.IsKeyJustReleased(MenuControls::MenuCancel) || controls.IsKeyJustPressed(MenuControls::MenuCancel) ||
        controls.IsKeyJustReleased(MenuControls::MenuUp) || controls.IsKeyJustPressed(MenuControls::MenuUp) ||
        controls.IsKeyJustReleased(MenuControls::MenuDown) || controls.IsKeyJustPressed(MenuControls::MenuDown) ||
        controls.IsKeyJustReleased(MenuControls::MenuLeft) || controls.IsKeyJustPressed(MenuControls::MenuLeft) ||
        controls.IsKeyJustReleased(MenuControls::MenuRight) || controls.IsKeyJustPressed(MenuControls::MenuRight)
       // PAD::IS_DISABLED_CONTROL_PRESSED(0, ControlPhoneSelect) || PAD::IS_DISABLED_CONTROL_JUST_RELEASED(0, ControlPhoneSelect) ||
       // PAD::IS_DISABLED_CONTROL_PRESSED(0, ControlPhoneCancel) || PAD::IS_DISABLED_CONTROL_JUST_RELEASED(0, ControlPhoneCancel) ||
       // PAD::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendUp) || PAD::IS_DISABLED_CONTROL_JUST_RELEASED(0, ControlFrontendUp) ||
       // PAD::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendDown) || PAD::IS_DISABLED_CONTROL_JUST_RELEASED(0, ControlFrontendDown) ||
       // PAD::IS_DISABLED_CONTROL_PRESSED(0, ControlPhoneLeft) || PAD::IS_DISABLED_CONTROL_JUST_RELEASED(0, ControlPhoneLeft) ||
       // PAD::IS_DISABLED_CONTROL_PRESSED(0, ControlPhoneRight) || PAD::IS_DISABLED_CONTROL_JUST_RELEASED(0, ControlPhoneRight)
        ) {
        menuTime = menuTimeRepeat;
    }

    for (unsigned i = 1; i < menuTimeDelays.size(); ++i) {
        if (controls.IsKeyDownFor(MenuControls::MenuUp, (i+1) * menuTimeRepeat) ||
            controls.IsKeyDownFor(MenuControls::MenuDown, (i + 1) * menuTimeRepeat) ||
            controls.IsKeyDownFor(MenuControls::MenuLeft, (i + 1) * menuTimeRepeat) ||
            controls.IsKeyDownFor(MenuControls::MenuRight, (i + 1) * menuTimeRepeat)) {
            menuTime = menuTimeDelays[i]; 
        }
    }
}

void Menu::OpenMenu() {
    if (menulevel == 0) {
        changeMenu("mainmenu");
        updateScreenSize();
        if (onMain) {
            onMain();
        }
        //instructionalButtonsScaleform.Request("instructional_buttons");
        //while (!instructionalButtonsScaleform.IsLoaded()) WAIT(0);
    }
}

void Menu::CloseMenu() {
    while (menulevel > 0) {
        backMenu();
    }
    if (onExit) onExit();
}

const MenuControls &Menu::GetControls() {
    return controls;
}

bool Menu::IsThisOpen() {
    return menulevel > 0;
}


std::vector<std::string> Menu::splitString(float maxWidth, const std::string& details, float scale, int font) {
    std::vector<std::string> splitLines;

    std::vector<std::string> words = split(details, ' ');

    std::string line;
    for (auto it = words.begin(); it != words.end(); ++it) {
        line += *it + ' ';
        if (std::next(it) == words.end()) {
            splitLines.push_back(line);
        }
    }

    return splitLines;
}

void Menu::drawText(const std::string& text, int font, float x, float y, float pUnknown, float scale, Color color, int justify) {
    HUD::SET_TEXT_SCALE(scale, scale);
    HUD::_SET_TEXT_COLOR(color.R, color.G, color.B, color.A);
    HUD::SET_TEXT_CENTRE(justify % 2 == 0);
    auto str = MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", text.c_str());
    HUD::_DRAW_TEXT(str, x, y);
}

void Menu::drawRect(float x, float y, float width, float height, Color color) {
    GRAPHICS::DRAW_RECT(x, y, width, height, color.R, color.G, color.B, color.A, 0, 0);
}

void Menu::drawSprite(const std::string& textureDict, const std::string& textureName, float x, float y, float width, 
                      float height, float rotation, Color color) {
        GRAPHICS::DRAW_SPRITE((char *)textureDict.c_str(), (char *)textureName.c_str(), x, y, width, height, rotation, color.R, color.G, color.B, color.A, 0);
}

void Menu::drawOptionPlusTitle(const std::string& title) {
    float newSize;
    std::string newTitle = title;
    fitTitle(newTitle, newSize, titleTextSize);

    float extrax = menuX + menuWidth;

    float titletexty = menuY + titleTextOffset;
    float titley = menuY + titleTextureOffset;

    textDraws.push_back(
        [=]() { drawText(newTitle, titleFont, extrax, titletexty, newSize, newSize, titleTextColor, 0); }
    );

    backgroundSpriteDraws.push_back(
        [=]() { drawSprite(textureDicts[titleTextureIndex], textureNames[titleTextureIndex],
        extrax, titley, menuWidth, titleHeight, 180.0f, titleBackgroundColor); }
    );
    
}

void Menu::drawOptionPlusImage(const std::string& extra, float &finalHeight) {
    int imgHandle;
    int imgWidth; 
    int imgHeight;
    std::string scanFormat = ImagePrefix + "%dW%dH%d";
    int nParams = sscanf_s(extra.c_str(), scanFormat.c_str(), &imgHandle, &imgWidth, &imgHeight);
    if (nParams != 3) {
        std::string errTxt = "Format error: " + extra;
        textDraws.push_back(
            [=]() { drawText(
                      errTxt, optionsFont, menuX + menuWidth / 2.0f + menuTextMargin, finalHeight + (menuY + headerHeight), optionTextSize, optionTextSize, optionsTextColor, 1); });
        finalHeight += optionHeight;
        return;
    }
    float drawWidth = menuWidth - 2.0f * menuTextMargin;
    float drawHeight = static_cast<float>(imgHeight) * (drawWidth / static_cast<float>(imgWidth));
    float imgXpos = (menuX + menuWidth / 2.0f + menuTextMargin);
    float imgYpos = finalHeight + (menuY + headerHeight) + menuTextMargin;


    float ar = 1.77778f;

    // game allows max 16/9 ratio for UI elements
    if (ar > 16.0f / 9.0f) {
        imgXpos += (ar - 16.0f / 9.0f) / (2.0f * ar);
    }
                  // screen correct, rgba
    finalHeight += drawHeight * ar + 2.0f * menuTextMargin;
}

void Menu::drawOptionPlusSprite(const std::string& extra, float &finalHeight) {
    const unsigned max_sz = 128;
    char dict[max_sz];
    char name[max_sz];
    int imgWidth;
    int imgHeight;
    std::string scanFormat = SpritePrefix + "%s %s W%dH%d";
    int nParams = sscanf_s(extra.c_str(), scanFormat.c_str(), dict, max_sz, name, max_sz, &imgWidth, &imgHeight);
    if (nParams != 4) {
        std::string errTxt = "Format error: " + extra;
        textDraws.push_back(
            [=]() { drawText(
                      errTxt, optionsFont, menuX + menuWidth / 2.0f + menuTextMargin, finalHeight + (menuY + headerHeight), optionTextSize, optionTextSize, optionsTextColor, 1); });
        finalHeight += optionHeight;
        return;
    }
    float drawWidth = menuWidth - 2.0f * menuTextMargin;
    float drawHeight = static_cast<float>(imgHeight) * (drawWidth / static_cast<float>(imgWidth)) * 1.777778f;
    float imgXpos = menuX + menuWidth / 2.0f + drawWidth / 2.0f + menuTextMargin;
    float imgYpos = finalHeight + drawHeight/2.0f + (menuY + headerHeight) + menuTextMargin;
            
    foregroundSpriteCalls.push_back(
        [=]() { drawSprite(std::string(dict), std::string(name),
                  imgXpos, imgYpos, drawWidth, drawHeight, 0.0f, titleTextColor); }
    );
            
    finalHeight += drawHeight + 2.0f * menuTextMargin;
}

void Menu::drawOptionPlusText(const std::string& extra, float &finalHeight) {
    std::vector<std::string> splitExtra;
    const float big_ass_Chalet_London_mult = optionsFont == 0 ? 0.75f : 1.0f;
    auto splitLines = splitString(menuWidth, extra, optionTextSize * big_ass_Chalet_London_mult, optionsFont);
    splitExtra.insert(std::end(splitExtra), std::begin(splitLines), std::end(splitLines));
    float heightModifier = 0.8f;
    for (size_t line = 0; line < splitExtra.size(); line++) {
        textDraws.push_back(
            [=]() { drawText(
                      splitExtra[line], optionsFont, 
                      menuX + menuWidth / 2.0f + menuTextMargin, finalHeight + (menuY + headerHeight) + line * optionHeight * heightModifier,
                      optionTextSize, optionTextSize, 
                      optionsTextColor, 1); });
    }
    finalHeight += splitExtra.size() * optionHeight * heightModifier;
}

void Menu::drawOptionPlusExtras(const std::vector<std::string>& extras, std::string title) {
    float extrax = menuX + menuWidth;
    drawOptionPlusTitle(title);

    if (headerHeight == titleHeight + subtitleHeight) {
        float subtitleY = subtitleTextureOffset + menuY + titleHeight;
        backgroundRectDraws.push_back(
            [=]() { drawRect(extrax, subtitleY, menuWidth, subtitleHeight, solidBlack); }
        );
    }

    float finalHeight = 0;

    for (const auto& extra : extras) {
        if (extra.compare(0, ImagePrefix.size(), ImagePrefix) == 0) {
            drawOptionPlusImage(extra, finalHeight);
        }
        else if (extra.compare(0, SpritePrefix.size(), SpritePrefix) == 0) {
            drawOptionPlusSprite(extra, finalHeight);
        }
        else {
            drawOptionPlusText(extra, finalHeight);
        }
    }

    backgroundSpriteDraws.push_back(
        [=]() { drawSprite(textureDicts[backgTextureIndex], textureNames[backgTextureIndex],
        extrax, menuY + headerHeight + finalHeight / 2.0f, menuWidth, finalHeight, 0.0f, optionsBackgroundColor); }
    );
}

void Menu::drawMenuDetails(const std::vector<std::string>& details, float y) {
    std::vector<std::string> splitDetails;
    const float big_ass_Chalet_London_mult = optionsFont == 0 ? 0.75f : 1.0f;
    for (const auto& detailLine : details) {
        auto splitLines = splitString(menuWidth, detailLine, optionTextSize * big_ass_Chalet_London_mult, optionsFont);
        splitDetails.insert(std::end(splitDetails), std::begin(splitLines), std::end(splitLines));
    }

    for (unsigned i = 0; i < splitDetails.size(); ++i) {
        textDraws.push_back(
            [=]() { drawText(
            splitDetails[i], optionsFont, (menuX - menuWidth / 2.0f) + menuTextMargin, i * detailLineHeight + y, optionTextSize, optionTextSize, optionsTextColor, 1); });
    }

    // The thin line
    backgroundRectDraws.push_back([=]() { drawRect(
                                  menuX, y, menuWidth, optionHeight / 16.0f, solidBlack); });

    float boxHeight = (splitDetails.size() * detailLineHeight) + (optionHeight - detailLineHeight);

    backgroundSpriteDraws.push_back(
        [=]() { drawSprite(textureDicts[backgTextureIndex], textureNames[backgTextureIndex],
        menuX, y + boxHeight / 2, menuWidth, boxHeight, 0.0f, optionsBackgroundColor); });
}

void Menu::drawOptionValue(const std::string& printVar, bool highlighted, int items) {
    float indicatorHeight = totalHeight - optionHeight;

    std::string leftArrow = "< ";
    std::string rightArrow = " >";
    if (items == 0) {
        leftArrow = rightArrow = "";
    }
    // Non-scroll
    if (currentoption <= maxDisplay && optioncount <= maxDisplay) {
        textDraws.push_back([=]() { drawText(leftArrow + printVar + rightArrow, optionsFont,
                                      menuX + menuWidth / 2.0f - optionRightMargin,
                                      indicatorHeight + menuY,
                                      optionTextSize, optionTextSize,
                                      highlighted ? optionsTextSelectColor : optionsTextColor, 2); });
    }
    // Scroll
    else if (optioncount > currentoption - maxDisplay && optioncount <= currentoption) {
        int optioncount_ = optioncount;
        textDraws.push_back([=]() { drawText(leftArrow + printVar + rightArrow, optionsFont,
                                      menuX + menuWidth / 2.0f - optionRightMargin,
                                      menuY + headerHeight + (optioncount_ - (currentoption - maxDisplay + 1)) * optionHeight,
                                      optionTextSize, optionTextSize,
                                      highlighted ? optionsTextSelectColor : optionsTextColor, 2); });
    }
}

/*
 * Section Actions/Input
 */
void Menu::changeMenu(const std::string& menuname) {
    printf("\n changeMenu");
    currentmenu[menulevel] = actualmenu;
    lastoption[actualmenu] = currentoption;
    menulevel++;
    actualmenu = menuname;
    previousoption = currentoption;
    currentoption = getWithDef(lastoption, actualmenu, 1);
    menuBeep();
    resetButtonStates();
}

void Menu::nextOption() {
    printf("\n nextOption");

    previousoption = currentoption;
    if (currentoption < optioncount)
        currentoption++;
    else
        currentoption = 1;
    if (menulevel > 0) {
        menuBeep();
    }
    resetButtonStates();
}

void Menu::previousOption() {
    printf("\n previousOption");

    previousoption = currentoption;
    if (currentoption > 1)
        currentoption--;
    else
        currentoption = optioncount;
    if (menulevel > 0) {
        menuBeep();
    }
    resetButtonStates();
}

void Menu::backMenu() {
    printf("\n backMenu");

    if (menulevel > 0) {
        menuBeep();
    }
    lastoption[actualmenu] = currentoption;
    menulevel--;
    actualmenu = currentmenu[menulevel];
    previousoption = currentoption;
    currentoption = getWithDef(lastoption, actualmenu, 1);

}

void Menu::menuBeep() {
  //  AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", 0);
}

void Menu::resetButtonStates() {
    optionpress = false;
    leftpress = false;
    rightpress = false;
    uppress = false;
    downpress = false;
}

void Menu::enableKeysOnce(bool enable) {

}

void Menu::hideHUDComponents() {
 
}

void Menu::disableKeys() {
 
}

void Menu::processMenuNav() {

    if (controls.IsKeyJustPressed(MenuControls::MenuKey)) {
          printf("\n menuKey Pressed");
        if (menulevel == 0) {
            OpenMenu();
            printf("\n opening");
        }
        else {
            printf("\n closing");
            CloseMenu();
            enableKeysOnce(true);
            if (onExit) {
                onExit();
            }
        }
        delay = GetTickCount64();
        return;
    }
    if (controls.IsKeyJustPressed(MenuControls::MenuCancel)) {
        if (menulevel > 0) {
            if (menulevel == 1) {
                enableKeysOnce(true);
                if (onExit) {
                    onExit();
                }
            }
            backMenu();

        }
        delay = GetTickCount64();
    }
    if (controls.IsKeyJustPressed(MenuControls::MenuSelect)) {
        if (menulevel > 0) {
            menuBeep();
        }
        optionpress = true;
        delay = GetTickCount64();
    }
    if (controls.IsKeyPressed(MenuControls::MenuDown)) {
        nextOption();
        delay = GetTickCount64();
        downpress = true;
    }
    if (controls.IsKeyPressed(MenuControls::MenuUp)) {
        previousOption();
        delay = GetTickCount64();
        uppress = true;
    }
    if (controls.IsKeyPressed(MenuControls::MenuLeft)) {
        if (menulevel > 0) {
            menuBeep();
        }
        leftpress = true;
        delay = GetTickCount64();
    }
    if (controls.IsKeyPressed(MenuControls::MenuRight)) {
        if (menulevel > 0) {
            menuBeep();
        }
        rightpress = true;
        delay = GetTickCount64();
    }
}


void Menu::updateScreenSize() {
    aspectR = (16.0f / 9.0f)/1.777778f;
    menuWidth = menuWidthOriginal*aspectR;
}

}
