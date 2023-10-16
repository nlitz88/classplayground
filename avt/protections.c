#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

/**
 * Type         : protection value field. Really just an unsigned 32-bit integer.
 * Mainly typedeffing to avoid having to change up dimensions later in case the
 * protection buffer size changes.
*/
typedef uint32_t swar_prot_val_field;
const uint8_t swar_prot_val_field_size = 18;


/**
 * Function     : swar_prot_mask_packet
 * Description  : Applies a bitmask specific to the steering angle request packet.
 * Ensures that all bits to the left of the <swar_prot_val_field_size> bits are
 * "zerod out."
 * NOTE: this will need to be updated according to the protection value field/packet
 * size (currently configured for an 18-bit protection value packet).
*/
static inline void swar_prot_mask_packet(swar_prot_val_field *SWAR_packet_buf) {

    *SWAR_packet_buf = *SWAR_packet_buf & 0x0003ffff;

}


/**
 * Function     : take_twos_comp
 * 
 * Description  : This function will perform the "two's complement operation" on the
 * provided input number. This is NOT to be confused as a function that EXPRESSES the
 * provided number using the two's complement representation/notation. This function
 * is strictly performing the two's complement operation that conists of a.)
 * inverting the input number and b.) adding one to it.
 * 
 * Inputs       : num 32-bit field. If appropriate for your application,
 * sign-extend your number externally, then call this function.
 * 
 * Outputs      : the result of inverting num and adding one to it.
 *              
*/
static inline swar_prot_val_field swar_take_twos_comp(swar_prot_val_field num) {

    return (swar_prot_val_field)(~num)+1;

}

/**
 * NOTE: this function isn't quite where I want it to be just yet--but starting.
*/
static inline void pack_swar_protection_value(bool SWAR_ReqAct, bool SWAR_ReqValid, int16_t SWAR_ReqSA, 
                                                uint8_t SWAR_ARC, swar_prot_val_field* SWAR_packet_buf) {

    // Create a new protection value field to build up the protection value packet
    // in.
    swar_prot_val_field SWAR_packet = 0x0;

    // First, shift the SWAR_ReqAct and SWAR_ReqValid bools into swar_prot_val_field
    // fields (in their respecitve positions).
    SWAR_packet = SWAR_packet | SWAR_ReqAct << (swar_prot_val_field_size - 1);
    SWAR_packet = SWAR_packet | SWAR_ReqValid << (swar_prot_val_field_size - 2);
    
    // The following line will place the sign-magnitude requested steering angle in
    // the right half of the swar_prot_val_field, padded with zeros on the left.
    SWAR_packet = SWAR_packet | (uint16_t)SWAR_ReqSA;
    

    // Next, add the steering angle request "alive rolling count" to the packet.
    // NOTE: BECAUSE both operands of this addition are UNSIGNED, any OVERFLOW will
    // be IGNORED! (which is what we want for this addition, apparently).
    SWAR_packet += SWAR_ARC;

    // Finally, perform the two's complement operation (invert then add one) on the
    // steering angle request packet.
    SWAR_packet = swar_take_twos_comp(SWAR_packet);

    // Make sure all bits to the left of the 18 comprising our field are zerod out.
    swar_prot_mask_packet(SWAR_packet);

    // Write the completed packet to the provided requested steering angle packet
    // buffer.
    *SWAR_packet_buf = SWAR_packet;

    return;

}


/**
 * Function     : main
 * Description  : 
 * Inputs       :
*/
int main(int argc, char *argv[]) {

    int8_t negative_num = -5;
    printf("8-bit signed integer: %#08x", negative_num);
    uint8_t unsigned_version = negative_num;
    printf("\n8-bit unsigned version: %#08x", unsigned_version);
    uint16_t unsigned_version_16 = negative_num;
    printf("\n16-bit unsigned version: %#08x", unsigned_version_16);

    // Okay, so whenever we cast the signed integer to the unsigned integer of the
    // same size, the sign just DOESN'T GET EXTENDED!
    // I.e., what basically happens is the "integer field" is storing the 2's
    // complement representation of that negative number. As expected, within that
    // signed integer variable type, the sign of that number is extended the whole
    // way out to the left! (the 1 is extended to the left).
    // When we assign that value to the unsigned value, we are quite literally just
    // ORING the current bits in the intger field (that signed extended two's
    // complement representation) with the 0 bits in the unsigned field! AND, in the
    // unsigned field, no sign extension occurs!

    int8_t neg_num_to_or = -5;
    uint16_t unsigned_field_16_to_or = 0x0000;
    printf("\nORing 8-bit signed with 16-bit unsigned 0's: %#08x", neg_num_to_or | unsigned_field_16_to_or);

    // Now, the above example shows how, if we JUST or the negative 8-bit number with
    // the 16-bit unsigned field, it happens that the negative 8-bit number will get
    // promoted to a signed 16-bit field to match the length of the unsigned 16-bit
    // field. THEN, because of this, we get the sign extension included in the 16-bit
    // field.
    
    printf("\nORing 8-bit signed with 16-bit unsigned 0's, but cast first: %#08x", (uint8_t)neg_num_to_or | unsigned_field_16_to_or);
    // SO, in the above, if we FIRST cast the signed 8-bit number to an unsigned
    // 8-bit number, we only get the 8-bits including the sign-extension, not all 16!

    // SO, for our intents and purposes, I was just interested in knowing that when I
    // cast a signed number to an unsigned number, I'm just going to get the same
    // bits that are in the signed variable. It's just that, if I'm working with the
    // unsigned variable, it won't do sign extension and all that.


    // Set up parameters for SWAR protection value packet.
    swar_prot_val_field swar_prot_val_packet = 0;
    int16_t steering_angle = 0;
    bool request_active = 0;
    bool request_valid = 0;

    printf("\nGenerating SWAR protection value packets for Steering angle %d", steering_angle);
    // For when Req_Active is LOW
    request_active = 0;
    printf("\nGenerating protection values for Req_Active LOW");
    for (int rc=0; rc<4; ++rc) {
        
        pack_swar_protection_value(request_active, request_valid, steering_angle, rc, &swar_prot_val_packet);
        printf("\n\tSWAR protection value for RC %d:"
                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " "
                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " ",
                 rc, 
                 BYTE_TO_BINARY(swar_prot_val_packet>>24),
                 BYTE_TO_BINARY(swar_prot_val_packet>>16),
                 BYTE_TO_BINARY(swar_prot_val_packet>>8),
                 BYTE_TO_BINARY(swar_prot_val_packet));
    }
    
    // For when Req_Active is HIGH
    request_active = 1;
    printf("\nGenerating protection values for Req_Active HIGH");
    for (int rc=0; rc<4; ++rc) {
        pack_swar_protection_value(request_active, request_valid, steering_angle, rc, &swar_prot_val_packet);
        printf("\n\tSWAR protection value for RC %d:"
                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " "
                BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " ",
                 rc, 
                 BYTE_TO_BINARY(swar_prot_val_packet>>24),
                 BYTE_TO_BINARY(swar_prot_val_packet>>16),
                 BYTE_TO_BINARY(swar_prot_val_packet>>8),
                 BYTE_TO_BINARY(swar_prot_val_packet));
    }

    return 0;
}

